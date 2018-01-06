#include <iostream>
#include <string>
#include <cstdint>
#include <cmath>

#include "cpu.hpp"

int main() {
	std::string input;
	bool quit = false;

	cpu test_cpu;
	//test_cpu.execute_instruction(0x31, 0xFE, 0xFF);
	test_cpu.execute_instruction(0x3E, 123);
	test_cpu.execute_instruction(0x32, 0x00, 0x00);

	print_registers(test_cpu);
	print_memory(test_cpu, 0x0000, 4);

	while(!quit) {
		std::cin >> input;

		if(input == "q") quit = true;
	}
}