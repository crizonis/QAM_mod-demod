#include "generator_gauss_noise.h"

#include <cmath>
#include <ctime>

Generator_gauss_noise::Generator_gauss_noise() { srand((unsigned int)time(0)); }

Generator_gauss_noise::Generator_gauss_noise(double noise) {
  srand((unsigned int)time(0));
  noise_ = noise;
}

void Generator_gauss_noise::add_noise(complex<double> *value) {
  double u, v, s;
  do {
    u = ((double)rand() / RAND_MAX) * 2 - 1;
    v = ((double)rand() / RAND_MAX) * 2 - 1;
    s = u * u + v * v;
  } while (s > 1);
  double delta_1 = u * sqrt((-2 * log(s)) / s);
  double delta_2 = v * sqrt((-2 * log(s)) / s);
  (*value).real(real(*value) + delta_1 * noise_);
  (*value).imag(imag(*value) + delta_2 * noise_);
}
complex<double> Generator_gauss_noise::add_noise(complex<double> value) {
  double delta1 = (((double)rand() / RAND_MAX) * 2 * noise_) - noise_;
  double delta2 = (((double)rand() / RAND_MAX) * 2 * noise_) - noise_;
  complex<double> result(real(value) + delta1, imag(value) + delta2);
  return result;
}

bool Generator_gauss_noise::make_noise(ifstream &src, ofstream &dst) {
  if (!src.is_open() || !dst.is_open())
    return false;
  complex<double> curr_value;
  while (src.read(reinterpret_cast<char *>(&curr_value), sizeof(curr_value))) {
    add_noise(&curr_value);
    dst.write(reinterpret_cast<char *>(&curr_value), sizeof(curr_value));
  }
  return true;
}

bool Generator_gauss_noise::make_noise(vector<complex<double>> src,
                                       vector<complex<double>> dst) {
  if (src.size() == 0)
    return false;
  dst.clear();
  for (vector<complex<double>>::const_iterator elem = src.begin();
       elem != src.end(); ++elem) {
    dst.push_back(add_noise(*elem));
  }
  return true;
}

bool Generator_gauss_noise::make_noise(vector<complex<double>> src) {
  if (src.size() == 0)
    return false;
  for (vector<complex<double>>::const_iterator elem = src.begin();
       elem != src.end(); ++elem) {
    add_noise((complex<double> *)elem._Ptr);
  }
  return true;
}