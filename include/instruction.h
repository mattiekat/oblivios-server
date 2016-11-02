#pragma once

#include "opcode.h"
#include "location.h"

enum class AccessMode : uint8_t { DIRECT, RELATIVE };

/**
 * This namespace provides functions useful for decoding instructions residing in the RAM.
 * @note This assumes that the RAM is addressed for all valid uint16_t values, i.e. 0 to 2^16 -1.
 * @param ram Pointer to the beginning of RAM
 * @param addr Address of the start of the instruction in RAM
 */
namespace Instruction {
    inline OPCode getOPCode(const uint8_t* ram, uint16_t addr) {
        return OPCodeFromInt(ram[addr] >> 2);
    }

    inline AccessMode getArg1Mode(const uint8_t* ram, uint16_t addr) {
        //if the value is 1, return relative, otherwise direct
        return ((ram[addr] & 0x02) >> 1) ? AccessMode::RELATIVE : AccessMode::DIRECT;
    }

    inline AccessMode getArg2Mode(const uint8_t* ram, uint16_t addr) {
        //if the value is 1, return relative, otherwise direct
        return (ram[addr] & 0x01) ? AccessMode::RELATIVE : AccessMode::DIRECT;
    }

    inline Location getArg1Loc(const uint8_t* ram, uint16_t addr) {
        return LocationFromInt(ram[addr + 1] & 0x0F, false);
    }

    inline Location getArg2Loc(const uint8_t* ram, uint16_t addr) {
        return LocationFromInt((ram[addr + 1] & 0xF0) >> 4, true);
    }

    inline uint8_t numImds(const uint8_t* ram, uint16_t addr) {
        uint8_t count = 0;
        Location t = getArg1Loc(ram, addr);
        if(t == Location::IMD || t == Location::PIMD) ++count;
        t = getArg2Loc(ram, addr);
        if(t == Location::IMD || t == Location::PIMD) ++count;
        return count;
    }

    /**
     * Gets the address of the immediate for an argument.
     * @todo support more than 2 args
     * @param ram Pointer to the beginning of RAM
     * @param addr Address of the start of the instruction in RAM
     * @param argn The argument number, with 1 being the first argument and 2 being the second
     * @return addr+0 if no immediates exist, addr+2, or addr+4 depending on number of immedaites and position.
     */
    inline uint16_t getImdAddress(const uint8_t* ram, uint16_t addr, uint8_t argn) {
        if(!argn) return 0;
        uint8_t num_imds = Instruction::numImds(ram, addr);
        if(!num_imds) return 0;

        addr += 2; //at least 2
        if(argn > 1) addr += 2; //4 total

        return addr;
    }
};