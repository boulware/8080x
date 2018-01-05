#ifndef CPU_HPP
#define CPU_HPP

#include <iostream>
#include <cstdint>

// Instruction set
// NOP 0x00,
// MVI B 0x06,
// MVI C 0x0E,

const uint8_t REGISTER_COUNT = 8;
const char register_names[REGISTER_COUNT] = {'B', 'C', 'D', 'E', 'H', 'L', 'M', 'A'};

class cpu {
	friend void print_registers(cpu);
private:
	static constexpr uint8_t word_size = 8; // 8-bit

	uint16_t program_counter;
	uint16_t stack_pointer;
	uint8_t registers[REGISTER_COUNT]; // B, C, D, E, H, L, M, A
	uint16_t memory[word_size];

	// Get register pair indices.
	const void get_RP_indices(	unsigned int index,
								unsigned int& low_register,
								unsigned int& high_register);
public:
	cpu();
	void execute_instruction(uint8_t opCode, uint8_t arg1 = 0, uint8_t arg2 = 0);
};

void print_registers(cpu);

#endif