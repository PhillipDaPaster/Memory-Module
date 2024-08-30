#pragma once
#pragma warning(disable : 6001)
#pragma warning(disable : 4244)
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <tlhelp32.h>
#include <string>

#define INRANGE(x,a,b)    ((x) >= (a) && (x) <= (b))
#define GET_BITS(x)       (INRANGE((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xA) : (INRANGE((x), '0', '9') ? (x) - '0' : 0))
#define GET_BYTES(x)      ((GET_BITS((x)[0]) << 4) | GET_BITS((x)[1]))

class mem_mgmt {
public:
    HANDLE proc;

    struct mod_data {
        HMODULE mod;
        HMODULE base;
        uintptr_t size;
    };

    template <class T>
    T read_mem(uintptr_t addr) {
        T x;
        ReadProcessMemory(proc, reinterpret_cast<LPCVOID>(addr), &x, sizeof(x), NULL);
        return x;
    }

    bool read_raw_mem(uintptr_t addr, void* buffer, size_t size) {
        SIZE_T bytes_read;
        return ReadProcessMemory(proc, reinterpret_cast<LPCVOID>(addr), buffer, size, &bytes_read) && bytes_read == size;
    }

    template <class T>
    T write_mem(uintptr_t addr, T x) {
        WriteProcessMemory(proc, reinterpret_cast<LPVOID>(addr), &x, sizeof(x), NULL);
        return x;
    }

    std::wstring read_wstring(uintptr_t addr, size_t max_length = 256) {
        std::wstring result;
        wchar_t buffer[256]; 

        size_t length_to_read = (max_length < sizeof(buffer) / sizeof(wchar_t)) ? max_length : sizeof(buffer) / sizeof(wchar_t);

        if (read_raw_mem(addr, buffer, length_to_read * sizeof(wchar_t))) {
            buffer[length_to_read - 1] = L'\0'; 
            result.assign(buffer);
            result.erase(result.find_last_not_of(L'\0') + 1); 
        }

        return result;
    }

    std::string read_string(uintptr_t addr, size_t max_length = 256) {
        std::string result;
        char buffer[256];

        size_t length_to_read = (max_length < sizeof(buffer) / sizeof(char)) ? max_length : sizeof(buffer) / sizeof(char);

        if (read_raw_mem(addr, buffer, length_to_read)) {
            buffer[length_to_read - 1] = '\0';
            result.assign(buffer);
            result.erase(result.find_last_not_of('\0') + 1);
        }

        return result;
    }

    HANDLE get_proc(const wchar_t* name);
    uintptr_t get_pid(const wchar_t* name);
    HMODULE get_mod(uintptr_t pid, const wchar_t* name);
    HMODULE get_mod_base(DWORD pid, const std::string& name);
};

inline mem_mgmt m;
