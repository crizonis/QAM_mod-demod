#pragma once

#include <fstream>
#include <complex>
#include <vector>

using namespace std;

class Generator_gauss_noise
{
public:
	Generator_gauss_noise();
	Generator_gauss_noise(double noise);

	double get_noise() { return noise_; }
	void set_noise(double noise) { noise_ = noise; }

	void add_noise(complex<double>* value);
	complex<double> add_noise(complex<double> value);

	bool make_noise(ifstream& src, ofstream& dst);
	bool make_noise(vector<complex<double>> src, vector<complex<double>>  dst);
	bool make_noise(vector<complex<double>> src);

private:
	double noise_ = 0;
};

