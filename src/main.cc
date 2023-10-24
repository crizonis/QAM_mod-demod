#include <iostream>
#include <fstream>
#include <vector>
#include "./other_libs/ext_modules.h"

#define SIZE_OF_TEST_DATA 10000

using namespace std;
using namespace ext_modules;

int main() {
	ofstream file_output;
	write_binary_file("data.bin", file_output);
	if (!file_output.is_open()) return -1;
	generate_data(file_output, SIZE_OF_TEST_DATA);
	file_output.close();

	// simple check for modulation-demodulation (better with small size of test data)
	/*ifstream file_input;
	int is_error = check_all_qam(file_input, file_output);
	if (is_error) return is_error;*/
	
	stat_all_qam_with_noise();

	system("pause");
	return 0;
}
