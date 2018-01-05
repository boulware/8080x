#include "cpu.hpp"

#include <iostream>
#include <bitset>
#include <cassert>

const uint8_t RD_MASK = 0b00111000; // bitmask for a dst register
const uint8_t RS_MASK = 0b00000111; // bitmask for a src register
const uint8_t RP_MASK = 0b00110000; // bitmask for register pair
// 00=BC   (B:C as 16 bit register)
// 01=DE   (D:E as 16 bit register)
// 10=HL   (H:L as 16 bit register)
// 11=SP   (Stack pointer, refers to PSW (FLAGS:A) for PUSH/POP)

cpu::cpu() : program_counter(0), stack_pointer(0), registers{0} {
}

void cpu::execute_instruction(	uint8_t opCode, 
								uint8_t arg1,
								uint8_t arg2) {
	// MOV D,S   01DDDSSS
	// D <- S
	// Moves value from register S to register D
	if( (opCode & 0b11000000) == 0b01000000) {
		int destination = (opCode & RD_MASK) >> 3;
		int source = (opCode & RS_MASK) >> 0;

		if(destination == 0b110 && source == 0b110) {
			std::cout << "HLT op code sent, but halting is not yet implemented.\n";
		} else {
			registers[destination] = registers[source];
		}
	} else
	// MVI D,#8   00DDD110 db
	// D <- #8
	// Moves 8-bit literal into register D.
	if( (opCode & 0b11000111) == 0b00000110) {
		int destination = (opCode & RD_MASK) >> 3;

		registers[destination] = arg1;
	}
	// LXI RP,#16  00RP0001 lb hb
	// RP <- #16
	// Moves 16-bit literal into register pair RP.
	if( (opCode & 0b11001111) == 0b00000001) {
		unsigned int RP_index = (opCode & RP_MASK) >> 4; //BC=0,DE=1,HL=2,SP=3
		unsigned int low_destination, high_destination;

		if(RP_index == 3) { // destination is stack pointer.
			stack_pointer = (arg2 << 8) | (arg1 << 0);
		} else {
			get_RP_indices(RP_index, low_destination, high_destination);
			registers[low_destination] = arg1;
			registers[high_destination] = arg2;
		}
	}
}

inline
const void cpu::get_RP_indices(unsigned int index, unsigned int& low_register, unsigned int& high_register) {
	assert(index <= 2 && "Register pair index is out of range");
	high_register = (2 * index);
	low_register = 	(2 * index) + 1;
}

void print_registers(cpu Cpu) {
	std::cout << "PC = " << Cpu.program_counter << "\n";
	std::cout << "SP = " << Cpu.stack_pointer << "\n";
	for(int i = 0; i < REGISTER_COUNT; i++) {
		std::cout << register_names[i] << " = " << static_cast<int>(Cpu.registers[i]) << "\n";
	}
}