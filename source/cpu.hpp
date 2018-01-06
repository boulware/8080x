#ifndef CPU_HPP
#define CPU_HPP

#include <iostream>
#include <cstdint>
#include <cmath>
#include <map>

// Instruction set
// NOP 0x00,
// MVI B 0x06,
// MVI C 0x0E,

namespace {
	using Item = std::pair<int, char>;
	constexpr Item register_names[] = {
		{0, 'B'}, {1, 'C'},
		{2, 'D'}, {3, 'E'},
		{4, 'H'}, {5, 'L'},
		{6, 'M'}, {7, 'A'},
	};
	constexpr auto register_count = sizeof(register_names)/sizeof(register_names[0]);	
}

class cpu {
	friend void print_registers(cpu);
	friend void print_memory(cpu, uint16_t, unsigned int);
private:
	static constexpr auto word_size = 8u;
	static constexpr auto EIGHT_BIT = 256u;
	static constexpr auto SIXTEEN_BIT = 65536u;

	uint16_t program_counter;
	uint16_t stack_pointer;
	uint8_t registers[register_count]; // B, C, D, E, H, L, M, A
	uint16_t memory[SIXTEEN_BIT];

	// Get register pair indices.
	const void get_RP_indices(	unsigned int index,
								unsigned int& low_register,
								unsigned int& high_register);
public:
	cpu();
	void execute_instruction(uint8_t opCode, uint8_t arg1 = 0, uint8_t arg2 = 0);
	void zero_all();
};

void print_registers(cpu);
void print_memory(cpu, uint16_t, unsigned int);

#endif