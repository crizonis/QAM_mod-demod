#include "qam_modulator.h"

#include <complex>
#include <cstdint>

#ifndef QAM_BASES

#define QAM_BASES
#define QAM_BASE_4 4
#define QAM_BASE_16 16
#define QAM_BASE_64 64

#define Q_AMP_1 1  // amplitude
#define Q_AMP_2 3
#define Q_AMP_3 5
#define Q_AMP_4 7
#define Q_DELTA 2  // distance between points of QAM Constellation

#define log4_16 2;
#define log4_64 3;

#endif

using namespace modulator;

const int8_t Qam_modulator_4::arr_qam4_imag[] = {Q_AMP_1, Q_AMP_1, -Q_AMP_1, -Q_AMP_1};
const int8_t Qam_modulator_4::arr_qam4_real[] = {Q_AMP_1, -Q_AMP_1, Q_AMP_1, -Q_AMP_1};

const int8_t Qam_modulator_16::arr_qam16_real[] = {Q_AMP_2, -Q_AMP_2, Q_AMP_2, -Q_AMP_2, Q_AMP_1, -Q_AMP_1,
                                                   Q_AMP_1, -Q_AMP_1, Q_AMP_2, -Q_AMP_2, Q_AMP_2, -Q_AMP_2,
                                                   Q_AMP_1, -Q_AMP_1, Q_AMP_1, -Q_AMP_1};
const int8_t Qam_modulator_16::arr_qam16_imag[] = {Q_AMP_2,  Q_AMP_2,  -Q_AMP_2, -Q_AMP_2, Q_AMP_2,  Q_AMP_2,
                                                   -Q_AMP_2, -Q_AMP_2, Q_AMP_1,  Q_AMP_1,  -Q_AMP_1, -Q_AMP_1,
                                                   Q_AMP_1,  Q_AMP_1,  -Q_AMP_1, -Q_AMP_1};

const int8_t Qam_modulator_64::arr_qam64_real[] = {
    Q_AMP_4, -Q_AMP_4, Q_AMP_4, -Q_AMP_4, Q_AMP_1, -Q_AMP_1, Q_AMP_1, -Q_AMP_1, Q_AMP_4, -Q_AMP_4,
    Q_AMP_4, -Q_AMP_4, Q_AMP_1, -Q_AMP_1, Q_AMP_1, -Q_AMP_1, Q_AMP_3, -Q_AMP_3, Q_AMP_3, -Q_AMP_3,
    Q_AMP_2, -Q_AMP_2, Q_AMP_2, -Q_AMP_2, Q_AMP_3, -Q_AMP_3, Q_AMP_3, -Q_AMP_3, Q_AMP_2, -Q_AMP_2,
    Q_AMP_2, -Q_AMP_2, Q_AMP_4, -Q_AMP_4, Q_AMP_4, -Q_AMP_4, Q_AMP_1, -Q_AMP_1, Q_AMP_1, -Q_AMP_1,
    Q_AMP_4, -Q_AMP_4, Q_AMP_4, -Q_AMP_4, Q_AMP_1, -Q_AMP_1, Q_AMP_1, -Q_AMP_1, Q_AMP_3, -Q_AMP_3,
    Q_AMP_3, -Q_AMP_3, Q_AMP_2, -Q_AMP_2, Q_AMP_2, -Q_AMP_2, Q_AMP_3, -Q_AMP_3, Q_AMP_3, -Q_AMP_3,
    Q_AMP_2, -Q_AMP_2, Q_AMP_2, -Q_AMP_2};

const int8_t Qam_modulator_64::arr_qam64_imag[] = {
    Q_AMP_4,  Q_AMP_4,  -Q_AMP_4, -Q_AMP_4, Q_AMP_4,  Q_AMP_4,  -Q_AMP_4, -Q_AMP_4, Q_AMP_1,  Q_AMP_1,
    -Q_AMP_1, -Q_AMP_1, Q_AMP_1,  Q_AMP_1,  -Q_AMP_1, -Q_AMP_1, Q_AMP_4,  Q_AMP_4,  -Q_AMP_4, -Q_AMP_4,
    Q_AMP_4,  Q_AMP_4,  -Q_AMP_4, -Q_AMP_4, Q_AMP_1,  Q_AMP_1,  -Q_AMP_1, -Q_AMP_1, Q_AMP_1,  Q_AMP_1,
    -Q_AMP_1, -Q_AMP_1, Q_AMP_3,  Q_AMP_3,  -Q_AMP_3, -Q_AMP_3, Q_AMP_3,  Q_AMP_3,  -Q_AMP_3, -Q_AMP_3,
    Q_AMP_2,  Q_AMP_2,  -Q_AMP_2, -Q_AMP_2, Q_AMP_2,  Q_AMP_2,  -Q_AMP_2, -Q_AMP_2, Q_AMP_3,  Q_AMP_3,
    -Q_AMP_3, -Q_AMP_3, Q_AMP_3,  Q_AMP_3,  -Q_AMP_3, -Q_AMP_3, Q_AMP_2,  Q_AMP_2,  -Q_AMP_2, -Q_AMP_2,
    Q_AMP_2,  Q_AMP_2,  -Q_AMP_2, -Q_AMP_2};

void Qam_modulator::init_iofiles(ifstream& input, ofstream& output) {
  input_ = &input;
  output_ = &output;
}

void Qam_modulator_4::modulate() const {
  uint8_t one_byte;
  while (input_->read(reinterpret_cast<char*>(&one_byte), sizeof(one_byte))) {
    complex<double> value;
    for (int i = 0; i < 4; ++i) {
      uint8_t low_2_bits = one_byte & 0b11;
      value.real(arr_qam4_real[low_2_bits]);
      value.imag(arr_qam4_imag[low_2_bits]);
      one_byte >>= 2;
      output_->write(reinterpret_cast<char*>(&value), sizeof(value));
    }
  }
}

void Qam_modulator_16::modulate() const {
  uint8_t one_byte;
  while (input_->read(reinterpret_cast<char*>(&one_byte), sizeof(one_byte))) {
    complex<double> value;
    for (int i = 0; i < 2; ++i) {
      uint8_t low_4_bits = one_byte & 0xF;
      value.real(arr_qam16_real[low_4_bits]);
      value.imag(arr_qam16_imag[low_4_bits]);
      one_byte >>= 4;
      output_->write(reinterpret_cast<char*>(&value), sizeof(value));
    }
  }
}

void Qam_modulator_64::modulate() const {
  uint8_t one_byte, remainder;
  uint8_t stage = 0;
  while (input_->read(reinterpret_cast<char*>(&one_byte), sizeof(one_byte))) {
    uint8_t set_6_bits;
    if (stage == 0) {
      set_6_bits = one_byte >> 2;
      remainder = one_byte & 0b11;
      stage++;
    } else if (stage == 1) {
      set_6_bits = (remainder << 4) + (one_byte >> 4);
      remainder = one_byte & 0b1111;
      stage++;
    } else if (stage == 2) {
      set_6_bits = (remainder << 2) + (one_byte >> 6);
      complex<double> value(arr_qam64_real[set_6_bits], arr_qam64_imag[set_6_bits]);
      output_->write(reinterpret_cast<char*>(&value), sizeof(value));

      set_6_bits = one_byte & 0b111111;
      stage = 0;
    }

    complex<double> value(arr_qam64_real[set_6_bits], arr_qam64_imag[set_6_bits]);
    output_->write(reinterpret_cast<char*>(&value), sizeof(value));
  }

  if (stage == 1 || stage == 2) {
    if (stage == 1)
      remainder <<= 4;
    else
      remainder <<= 2;
    complex<double> value(arr_qam64_real[remainder], arr_qam64_imag[remainder]);
    output_->write(reinterpret_cast<char*>(&value), sizeof(value));
  }
}

Qam_modulator* Creator_Qam_4::FactoryMethod() const { return new Qam_modulator_4(); }

Qam_modulator* Creator_Qam_16::FactoryMethod() const { return new Qam_modulator_16(); }

Qam_modulator* Creator_Qam_64::FactoryMethod() const { return new Qam_modulator_64(); }
