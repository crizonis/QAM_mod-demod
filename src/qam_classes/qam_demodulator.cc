#include "qam_demodulator.h"
#include <complex>
#include <cstdint>

#ifndef QAM_BASES

#define QAM_BASES
#define QAM_BASE_4 4
#define QAM_BASE_16 16
#define QAM_BASE_64 64

#define Q_AMP_1 1	// amplitude
#define Q_AMP_2 3
#define Q_AMP_3 5
#define Q_AMP_4 7
#define Q_DELTA 2	// distance between points of QAM Constellation

#define LOG4_16 2
#define LOG4_64 3

#endif

//using namespace std;
using namespace demodulator;

struct borders{
	int8_t left;
	int8_t right;
};


const uint8_t Qam_demodulator_16::arr_quater_to_bits[4][4] = {	{ 0b00, 0b01, 0b10, 0b11 },
																	{ 0b01, 0b00, 0b11, 0b10 },
																	{ 0b10, 0b11, 0b00, 0b01 },
																	{ 0b11, 0b10, 0b01, 0b00 } };

const uint8_t Qam_demodulator_64::arr_quater_to_bits[4][4] = { { 0b00, 0b01, 0b10, 0b11 },
																	{ 0b01, 0b00, 0b11, 0b10 },
																	{ 0b10, 0b11, 0b00, 0b01 },
																	{ 0b11, 0b10, 0b01, 0b00 } };


void Qam_demodulator::init_iofiles(ifstream& input, ofstream& output) {
	input_ = &input;
	output_ = &output;
}

void Qam_demodulator_4::demodulate() const{
	complex<double> curr_value;
	uint8_t counter = 0;
	uint8_t value[4];
	while (input_->read((char*)&curr_value, sizeof(curr_value))) {
		value[counter] = complex_to_2_bits(curr_value);
		if (counter == 3) {
			uint8_t res = (value[3] << 6) + (value[2] << 4) + (value[1] << 2) + value[0];
			output_->write((char*)&res, sizeof(res));
			counter = 0;
		} else counter++;
	}
}

void Qam_demodulator_16::demodulate() const {
	complex<double> curr_value;
	uint8_t counter = 0;
	uint8_t value[2];
	while (input_->read((char*)&curr_value, sizeof(curr_value))) {
		value[counter] = complex_to_4_bits(curr_value);
		if (counter>0) {
			uint8_t res = (value[1] << 4) + value[0];
			output_->write((char*)&res, sizeof(res));
			counter = 0;
		}
		else counter++;
	}
	
}

void Qam_demodulator_64::demodulate() const {
	complex<double> curr_value;
	uint8_t counter = 0;
	uint8_t curr_byte;
	uint8_t stage = 0;
	while (input_->read((char*)&curr_value, sizeof(curr_value))) {
		uint8_t set_bits;
		if (stage == 0) {
			curr_byte = complex_to_6_bits(curr_value);
			stage++;
		}
		else if (stage == 1) {
			set_bits = complex_to_6_bits(curr_value);
			curr_byte = (curr_byte << 2) + (set_bits >> 4);
			output_->write((char*)& (curr_byte), sizeof(curr_byte));
			curr_byte = set_bits & 0xF;
			stage++;
		}
		else if (stage == 2) {
			set_bits = complex_to_6_bits(curr_value);
			curr_byte = (curr_byte << 4) + (set_bits >> 2);
			output_->write((char*)& (curr_byte), sizeof(curr_byte));
			curr_byte = set_bits & 0b11;
			stage++;
		}
		else if (stage == 3) {
			set_bits = complex_to_6_bits(curr_value);
			curr_byte = (curr_byte << 6) + set_bits;
			output_->write((char*)& (curr_byte), sizeof(curr_byte));
			stage = 0;
		}
	}
}

inline uint8_t Qam_demodulator::get_grey_quarter(complex<double> value, int8_t o_real, int8_t o_imag) const {
	uint8_t result;
	if (value.imag() > o_imag)
		if (value.real() > o_real)
			result = 0b00;
		else
			result = 0b01;
	else
		if (value.real() > o_real)
			result = 0b10;
		else
			result = 0b11;
	return result;
}

inline uint8_t Qam_demodulator_4::complex_to_2_bits(complex<double> value) const {
	uint8_t result;
	result = get_grey_quarter(value, 0, 0);
	return result;
}

inline uint8_t Qam_demodulator_16::complex_to_4_bits(complex<double> value) const {
	uint8_t result = 0;
	uint8_t curr_2_bits, curr_quater, last_quater = 0;
	borders borders_real = { -Q_AMP_2 - (Q_DELTA / 2), Q_AMP_2 + (Q_DELTA / 2) };
	borders borders_imag = { -Q_AMP_2 - (Q_DELTA / 2), Q_AMP_2 + (Q_DELTA / 2) };
	for (int i = 0; i < LOG4_16; ++i) {
		curr_quater = get_grey_quarter(value, (borders_real.right + borders_real.left) / 2, (borders_imag.right + borders_imag.left) / 2);
		curr_2_bits = arr_quater_to_bits[last_quater][curr_quater];
		result = (curr_2_bits << 2 * i) + result;
		
		if (curr_quater == 0) {
			borders_real.left = (borders_real.right + borders_real.left) / 2;
			borders_imag.left = (borders_imag.right + borders_imag.left) / 2;
		}
		else if (curr_quater == 1) {
			borders_real.right = (borders_real.right + borders_real.left) / 2;
			borders_imag.left = (borders_imag.right + borders_imag.left) / 2;
		}
		else if (curr_quater == 2) {
			borders_real.left = (borders_real.right + borders_real.left) / 2;
			borders_imag.right = (borders_imag.right + borders_imag.left) / 2;
		}
		else if (curr_quater == 3) {
			borders_real.right = (borders_real.right + borders_real.left) / 2;
			borders_imag.right = (borders_imag.right + borders_imag.left) / 2;
		}
		last_quater = curr_quater;
	}
	return result;
}



inline uint8_t Qam_demodulator_64::complex_to_6_bits(complex<double> value) const {
	uint8_t result = 0;
	uint8_t curr_2_bits, curr_quater, last_quater = 0;
	borders borders_real = { -Q_AMP_4 - (Q_DELTA / 2), Q_AMP_4 + (Q_DELTA / 2) };
	borders borders_imag = { -Q_AMP_4 - (Q_DELTA / 2), Q_AMP_4 + (Q_DELTA / 2) };
	for (int i = 0; i < LOG4_64; ++i) {
		curr_quater = get_grey_quarter(value, (borders_real.right + borders_real.left) / 2, (borders_imag.right + borders_imag.left) / 2);
		curr_2_bits = arr_quater_to_bits[last_quater][curr_quater];
		result = (curr_2_bits << 2 * i) + result;

		if (curr_quater == 0) {
			borders_real.left = (borders_real.right + borders_real.left) / 2;
			borders_imag.left = (borders_imag.right + borders_imag.left) / 2;
		}
		else if (curr_quater == 1) {
			borders_real.right = (borders_real.right + borders_real.left) / 2;
			borders_imag.left = (borders_imag.right + borders_imag.left) / 2;
		}
		else if (curr_quater == 2) {
			borders_real.left = (borders_real.right + borders_real.left) / 2;
			borders_imag.right = (borders_imag.right + borders_imag.left) / 2;
		}
		else if (curr_quater == 3) {
			borders_real.right = (borders_real.right + borders_real.left) / 2;
			borders_imag.right = (borders_imag.right + borders_imag.left) / 2;
		}
		last_quater = curr_quater;
	}
	return result;
}





Qam_demodulator* Creator_Qam_4::FactoryMethod() const
{
	return new Qam_demodulator_4();
}

Qam_demodulator* Creator_Qam_16::FactoryMethod() const
{
	return new Qam_demodulator_16();
}

Qam_demodulator* Creator_Qam_64::FactoryMethod() const
{
	return new Qam_demodulator_64();
}

