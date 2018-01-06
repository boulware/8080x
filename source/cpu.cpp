#include "cpu.hpp"

#include <iostream>
#include <bitset>
#include <cassert>
#include <iomanip>

namespace {
	constexpr uint8_t RD_MASK = 0b00111000; // bitmask for a dst register
	constexpr uint8_t RS_MASK = 0b00000111; // bitmask for a src register
	constexpr uint8_t RP_MASK = 0b00110000; // bitmask for register pair
	// 00=BC   (B:C as 16 bit register)
	// 01=DE   (D:E as 16 bit register)
	// 10=HL   (H:L as 16 bit register)
	// 11=SP   (Stack pointer, refers to PSW (FLAGS:A) for PUSH/POP)

	static constexpr int getRegisterIndex(char name, int range = register_count) {
		return
			(range == 0) ? throw "Register name does not exist.":
			(register_names[range - 1].second == name) ? register_names[range - 1].first:
			getRegisterIndex(name, range - 1);
	}
	static constexpr char getRegisterName(int index, int range = register_count) {
		return
			(range == 0) ? throw "Register index does not exist.":
			(register_names[range - 1].first == index) ? register_names[range - 1].second:
			getRegisterName(index, range - 1);
	}
}

cpu::cpu() : program_counter(0), stack_pointer(0), registers{0} {
}

void cpu::execute_instruction(	uint8_t opCode, 
								uint8_t arg1,
								uint8_t arg2) {
	// MOV F,I   01FFFIII
	// F <- I
	// Moves value from register I to register F
	if( (opCode & 0b11000000) == 0b01000000) {
		int destination = (opCode & RD_MASK) >> 3;
		int source = (opCode & RS_MASK) >> 0;

		if(destination == 0b110 && source == 0b110) {
			std::cout << "HLT op code sent, but halting is not yet implemented.\n";
		} else {
			registers[destination] = registers[source];
		}
	} else 	
	// MVI R,#8   00RRR110 db
	// R <- #8
	// Moves 8-bit literal into register R.
	if( (opCode & 0b11000111) == 0b00000110) {
		int destination = (opCode & RD_MASK) >> 3;

		registers[destination] = arg1;
	} else
	// LXI RP,#16  00RP0001 lb hb
	// RP <- #16
	// Moves 16-bit literal into register pair RP.
	if((opCode & 0b11001111) == 0b00000001) {
		unsigned int RP_index = (opCode & RP_MASK) >> 4; //BC=0,DE=1,HL=2,SP=3
		unsigned int low_destination, high_destination;

		if(RP_index == 3) { // destination is stack pointer.
			stack_pointer = (arg2 << 8) | (arg1 << 0);
		} else {
			get_RP_indices(RP_index, low_destination, high_destination);
			registers[low_destination] = arg1;
			registers[high_destination] = arg2;
		}
	} else
	// LDA (#16)     00111010 lb hb
	// A <- (#16)
	// Load value from memory at 16-bit address into register A
	if(opCode == 0b00111010) {
		uint16_t address = (arg2 << 8) | (arg1 << 0);

		registers[getRegisterIndex('A')] = memory[address];
	} else
	// STA (#16)     00110010 lb hb
	// (#16) <- A
	// Load value from register A into memory at 16-bit address
	if(opCode == 0b00110010) {
		uint16_t address = (arg2 << 8) | (arg1 << 0);

		memory[address] = registers[getRegisterIndex('A')];
	}
}

inline
const void cpu::get_RP_indices(unsigned int index, unsigned int& low_register, unsigned int& high_register) {
	assert(index <= 2 && "Register pair index is out of range");
	high_register = (2 * index);
	low_register = 	(2 * index) + 1;
}

void cpu::zero_all() {
	program_counter = 0;
	stack_pointer = 0;
	for(auto& e : registers) e = 0;
	for(auto& e : memory) e = 0;
}

void print_registers(cpu Cpu) {
	std::cout << "PC = " << Cpu.program_counter << "\n";
	std::cout << "SP = " << Cpu.stack_pointer << "\n";
	for(int i = 0; i < register_count; i++) {
		std::cout << getRegisterName(i) << " = " << static_cast<int>(Cpu.registers[i]) << "\n";
	}
}

void print_memory(cpu Cpu, uint16_t address, unsigned int byte_count) {
	assert((address + byte_count < 65536) && "Tried to read emulated memory out of range.");

	std::cout 	<< "0x"
				<< std::setfill('0') << std::setw(2)
				<< std::hex	
				<< address << ": ";
	for(int i = address; i < address + byte_count; i++) {
		std::cout << Cpu.memory[i] << " ";
	}
}