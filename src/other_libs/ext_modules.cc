#include "ext_modules.h"
#include "../qam_classes/generator_gauss_noise.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdint>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

void ext_modules::generate_data(ofstream &source, unsigned int n) {
	srand((unsigned int)time(0));
	
	for (unsigned int i = 0; i < n; i++) {
		uint8_t one_byte = rand() % 256;
		source.write((char*)&one_byte, 1);
	}
}

void ext_modules::show_data(ifstream &source) {
	char one_byte;
	while (source.get(one_byte)) {
		cout << setw(5) << (int)(uint8_t)one_byte << " ";
	}
	cout << endl;
}

bool ext_modules::read_binary_file(const char* name, ifstream& fs) {
	fs.open(name, ios_base::binary);
	if (!fs.is_open())
	{
		cout << "file " << name << " cannot be open for read!\n";
	}
	return fs.is_open();
}

bool ext_modules::write_binary_file(const char* name, ofstream& fs) {
	fs.open(name, ios_base::binary | ios_base::trunc);
	if (!fs.is_open())
	{
		cout << "file " << name << " cannot be open for write!\n";
	}
	return fs.is_open();
}

bool ext_modules::read_text_file(const char* name, ifstream& fs) {
	fs.open(name);
	if (!fs.is_open())
	{
		cout << "file " << name << " cannot be open for read!\n";
	}
	return fs.is_open();
}

bool ext_modules::write_text_file(const char* name, ofstream& fs) {
	fs.open(name,  ios_base::trunc);
	if (!fs.is_open())
	{
		cout << "file " << name << " cannot be open for write!\n";
	}
	return fs.is_open();
}

bool ext_modules::make_modulate(ifstream& input, ofstream& output, modulator::Qam_modulator* modulator) {
	bool status = input.is_open() && output.is_open();
	if (status) {
		modulator->init_iofiles(input, output);
		modulator->modulate();
	}
	return status;
}

bool ext_modules::make_demodulate(ifstream& input, ofstream& output, demodulator::Qam_demodulator* demodulator) {
	bool status = input.is_open() && output.is_open();
	if (status) {
		demodulator->init_iofiles(input, output);
		demodulator->demodulate();
	}
	return status;
}

int ext_modules::check_all_qam(ifstream& input, ofstream& output) {
	cout << "before:" << endl;
	read_binary_file("data.bin", input);
	if (!input.is_open()) return -12;
	show_data(input);
	input.close();

	int is_error = modulate_demodulate_all_qam(input, output);
	if (is_error) return is_error;

	return 0;
}

int ext_modules::modulate_demodulate_all_qam(ifstream& input, ofstream& output) {
	modulator::Creator_Qam_4 creator_m_4;
	modulator::Creator_Qam_16 creator_m_16;
	modulator::Creator_Qam_64 creator_m_64;
	modulator::Creator* m_creators[] = { &creator_m_4, &creator_m_16, &creator_m_64 };

	demodulator::Creator_Qam_4 creator_d_4;
	demodulator::Creator_Qam_16 creator_d_16;
	demodulator::Creator_Qam_64 creator_d_64;
	demodulator::Creator* d_creators[] = { &creator_d_4, &creator_d_16, &creator_d_64 };

	char* signal_files[] = { "data_complex_4.bin", "data_complex_16.bin", "data_complex_64.bin" };
	char* result_files[] = { "data_after_QAM_4.bin", "data_after_QAM_16.bin", "data_after_QAM_64.bin" };
	char* stages[] = { "QAM_4 (QPSK)", "QAM_16", "QAM_64" };

	for (int i = 0; i < 3; ++i) {
		read_binary_file("data.bin", input);
		write_binary_file(signal_files[i], output);
		modulator::Qam_modulator* modulator = m_creators[i]->FactoryMethod();
		if (!make_modulate(input, output, modulator)) return -13;
		input.close();
		output.close();

		read_binary_file(signal_files[i], input);
		write_binary_file(result_files[i], output);
		demodulator::Qam_demodulator* demodulator = d_creators[i]->FactoryMethod();
		if (!make_demodulate(input, output, demodulator)) return -14;
		input.close();
		output.close();

		cout << "after " << stages[i] << endl;
		read_binary_file(result_files[i], input);
		if (!input.is_open()) return -15;
		show_data(input);
		input.close();
	}
	return 0;
}


void ext_modules::file_to_vector(ifstream& src, vector<uint8_t>& dst) {
	dst.clear();
	uint8_t curr_byte;
	while (src.read((char*)&curr_byte, sizeof(uint8_t))) {
		dst.push_back(curr_byte);
	}
}

uint8_t ext_modules::compare_bits_of_byte(uint8_t a, uint8_t b) {
	uint8_t total = 0;
	a ^= b;
	for (uint8_t i=0; i < 8; ++i)
		if ((a & (1 << i)) == 1) total++;	
	return total;
}

long ext_modules::compare_bits_of_vector(vector<uint8_t>& a, vector<uint8_t>& b) {
	vector<uint8_t>::const_iterator it_a = a.begin();
	vector<uint8_t>::const_iterator it_b = b.begin();
	long total = 0;
	for (; it_a != a.end() && it_b != b.end(); ++it_a, ++it_b) {
		total += compare_bits_of_byte(*it_a, *it_b);
	}
	return total;
}

int ext_modules::stat_qam_with_noise(modulator::Qam_modulator* modulator, 
									demodulator::Qam_demodulator* demodulator, 
									const char* prefix, double noise_limit) {
	ifstream input;
	ifstream input_2;
	ofstream output;
	ofstream result_file;
	string name = "./stat/stat_misses_" + string(prefix) + ".csv";
	write_text_file(name.c_str(), result_file);
	

	read_binary_file("data.bin", input);
	vector<uint8_t> data_before;
	file_to_vector(input, data_before);
	input.clear();
	input.seekg(ios::beg);

	write_binary_file("data_complex.bin", output);
	if (!make_modulate(input, output, modulator)) return -21;
	input.close();
	output.close();

	Generator_gauss_noise noiser = Generator_gauss_noise();
	read_binary_file("data_complex.bin", input);
	for (double noise = 0; noise - noise_limit < 1e-3; noise += 0.025) {
		input.clear();
		input.seekg(ios::beg);
		write_binary_file("data_complex_noised.bin", output);
		noiser.set_noise(noise);
		if (!noiser.make_noise(input, output)) return -22;
		output.close();

		if (fabs((noise / 0.2 - round(noise / 0.2))) < 1e-3) {
			read_binary_file("data_complex_noised.bin", input_2);
			string name = "./stat/stat_constellation_" + string(prefix) + "_" + to_string(noise) + ".csv";
			write_binary_file(name.c_str(), output);
			stat_constellation(input_2, output);
			input_2.close();
			output.close();
		}

		read_binary_file("data_complex_noised.bin", input_2);
		write_binary_file("data_after.bin", output);
		if (!make_demodulate(input_2, output, demodulator)) return -23;
		input_2.close();
		output.close();

		read_binary_file("data_after.bin", input_2);
		vector<uint8_t> data_after;
		file_to_vector(input_2, data_after);
		input_2.close();

		
		long diff = compare_bits_of_vector(data_before, data_after);
		cout << "noise: " << noise << "\t\t" << diff << endl;
		
		
		result_file << noise << ";" << diff << endl;
		
	}
	result_file.close();
	input.close();
	return 0;
}

int ext_modules::stat_all_qam_with_noise() {

	modulator::Creator_Qam_4 creator_m_4;
	modulator::Creator_Qam_16 creator_m_16;
	modulator::Creator_Qam_64 creator_m_64;
	modulator::Creator* m_creators[] = { &creator_m_4, &creator_m_16, &creator_m_64 };

	demodulator::Creator_Qam_4 creator_d_4;
	demodulator::Creator_Qam_16 creator_d_16;
	demodulator::Creator_Qam_64 creator_d_64;
	demodulator::Creator* d_creators[] = { &creator_d_4, &creator_d_16, &creator_d_64 };

	char* stages[] = { "QAM_4 (QPSK)", "QAM_16", "QAM_64" };
	for (int i = 0; i < 3; ++i) {
		cout << stages[i] << endl;
		modulator::Qam_modulator* modulator = m_creators[i]->FactoryMethod();
		demodulator::Qam_demodulator* demodulator = d_creators[i]->FactoryMethod();
		stat_qam_with_noise(modulator, demodulator, stages[i], 2);
	}
	
	return 0;
}

void ext_modules::stat_constellation(ifstream& src, ofstream& dst) {
	complex<double> curr_value;
	while (src.read((char*)&curr_value, sizeof(curr_value))) {
		dst << real(curr_value) << ";" << imag(curr_value) << endl;
	}
}