#include <iostream>
#include <string>
#include <cstdint>
#include <cmath>

#include "cpu.hpp"

int main() {
	std::string input;
	bool quit = false;

	cpu test_cpu;
	test_cpu.execute_instruction(0x31, 0xFE, 0xFF);

	print_registers(test_cpu);

	while(!quit) {
		std::cin >> input;

		if(input == "q") quit = true;
	}
}