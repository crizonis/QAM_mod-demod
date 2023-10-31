#pragma once

#include <complex>
#include <fstream>

using namespace std;

namespace demodulator {
class Qam_demodulator {
 public:
  virtual void demodulate() const = 0;
  void init_iofiles(ifstream& input, ofstream& output);

 protected:
  inline uint8_t get_grey_quarter(complex<double> value, int8_t o_real, int8_t o_imag) const;
  ifstream* input_;
  ofstream* output_;
};

class Qam_demodulator_4 : public Qam_demodulator {
 public:
  void demodulate() const override;
  inline uint8_t complex_to_2_bits(complex<double> value) const;
};

class Qam_demodulator_16 : public Qam_demodulator {
 public:
  void demodulate() const override;

 private:
  inline uint8_t complex_to_4_bits(complex<double> value) const;
  static const uint8_t arr_quater_to_bits[4][4];
};

class Qam_demodulator_64 : public Qam_demodulator {
 public:
  void demodulate() const override;

 private:
  inline uint8_t complex_to_6_bits(complex<double> value) const;
  static const uint8_t arr_quater_to_bits[4][4];
};

class Creator {
 public:
  virtual ~Creator(){};
  virtual Qam_demodulator* FactoryMethod() const = 0;
};

class Creator_Qam_4 : public Creator {
 public:
  Qam_demodulator* FactoryMethod() const override;
};

class Creator_Qam_16 : public Creator {
 public:
  Qam_demodulator* FactoryMethod() const override;
};

class Creator_Qam_64 : public Creator {
 public:
  Qam_demodulator* FactoryMethod() const override;
};
}  // namespace demodulator