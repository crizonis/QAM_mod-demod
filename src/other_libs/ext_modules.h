#pragma once
#include <fstream>
#include <vector>
#include "./qam_classes/qam_modulator.h"
#include "./qam_classes/qam_demodulator.h"
using namespace std;


namespace ext_modules {
	void generate_data(ofstream& source, unsigned int n);
	void show_data(ifstream& source);
	void stat_constellation(ifstream& src, ofstream& dst);
	bool read_binary_file(const char* name, ifstream& fs);
	bool write_binary_file(const char* name, ofstream& fs);
	bool read_text_file(const char* name, ifstream& fs);
	bool write_text_file(const char* name, ofstream& fs);
	bool make_modulate(ifstream& input, ofstream& output, modulator::Qam_modulator* modulator);
	bool make_demodulate(ifstream& input, ofstream& output, demodulator::Qam_demodulator* demodulator);
	int modulate_demodulate_all_qam(ifstream& input, ofstream& output);
	int check_all_qam(ifstream& input, ofstream& output);
	int stat_qam_with_noise(modulator::Qam_modulator* modulator, demodulator::Qam_demodulator* demodulator, const char* prefix, double noise_limit);
	int stat_all_qam_with_noise();
	void file_to_vector(ifstream& src, vector<uint8_t>& dst);
	uint8_t compare_bits_of_byte(uint8_t a, uint8_t b);
	long compare_bits_of_vector(vector<uint8_t>& a, vector<uint8_t>& b);
}