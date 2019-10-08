#include <iostream>
#include <vector>
#include <omp.h>

int main() {
	constexpr size_t vector_size = 100000000;
	std::vector<double> v1;
	std::vector<double> v2;
	v1.reserve(vector_size);
	v2.reserve(vector_size);

	for (size_t i = 0; i < vector_size; ++i) {
		v1.emplace_back(i % 6);
		v2.emplace_back(i % 4);
	}

	for (int num_threads : { 1,2,4 }) {
		auto start = omp_get_wtime();
		std::vector<double> s(num_threads, 0);
		double result = 0;
#pragma omp parallel num_threads(num_threads)
		{
			size_t thread_num = omp_get_thread_num();
			size_t range = vector_size % num_threads == 0 ?
				vector_size / num_threads : vector_size / num_threads + 1;
			for (size_t i = range * thread_num; i < range * (thread_num + 1) && i < vector_size; ++i)
				s[thread_num] += v1[i] * v2[i];
		}
		for (auto sum : s)
			result += sum;
		auto end = omp_get_wtime();
		std::cout << "Number of threads: " << num_threads;
		std::cout << "\nTime:\n";
		std::cout << end - start << '\n';
		std::cout << result << std::endl;
	}
	system("pause");
	return 0;
}