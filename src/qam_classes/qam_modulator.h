#pragma once

#define _USE_MATH_DEFINES
#include <cstdint>
#include <fstream>
using namespace std;

namespace modulator {
class Qam_modulator {
 public:
  virtual void modulate() const = 0;
  void init_iofiles(ifstream& input, ofstream& output);

 protected:
  ifstream* input_;
  ofstream* output_;
};

class Qam_modulator_4 : public Qam_modulator {
 public:
  void modulate() const override;

 private:
  static const int8_t arr_qam4_imag[];
  static const int8_t arr_qam4_real[];
};

class Qam_modulator_16 : public Qam_modulator {
 public:
  void modulate() const override;

 private:
  static const int8_t arr_qam16_real[];
  static const int8_t arr_qam16_imag[];
};

class Qam_modulator_64 : public Qam_modulator {
 public:
  void modulate() const override;

 private:
  static const int8_t arr_qam64_real[];
  static const int8_t arr_qam64_imag[];
};

class Creator {
 public:
  virtual ~Creator(){};
  virtual Qam_modulator* FactoryMethod() const = 0;
};

class Creator_Qam_4 : public Creator {
 public:
  Qam_modulator* FactoryMethod() const override;
};

class Creator_Qam_16 : public Creator {
 public:
  Qam_modulator* FactoryMethod() const override;
};

class Creator_Qam_64 : public Creator {
 public:
  Qam_modulator* FactoryMethod() const override;
};

}  // namespace modulator
