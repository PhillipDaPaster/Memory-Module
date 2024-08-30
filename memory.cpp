#include "memory.h"

HANDLE mem_mgmt::get_proc(const wchar_t* name) {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    uintptr_t process;
    PROCESSENTRY32W proc_entry;
    proc_entry.dwSize = sizeof(proc_entry);

    if (Process32FirstW(snapshot, &proc_entry)) {
        do {
            if (!_wcsicmp(proc_entry.szExeFile, name)) {
                process = proc_entry.th32ProcessID;
                CloseHandle(snapshot);
                proc = OpenProcess(PROCESS_ALL_ACCESS, false, process);
                return proc;
            }
        } while (Process32NextW(snapshot, &proc_entry));
    }

    CloseHandle(snapshot);
    return NULL;
}

uintptr_t mem_mgmt::get_pid(const wchar_t* name) {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    uintptr_t process;
    PROCESSENTRY32W proc_entry;
    proc_entry.dwSize = sizeof(proc_entry);

    if (Process32FirstW(snapshot, &proc_entry)) {
        do {
            if (!_wcsicmp(proc_entry.szExeFile, name)) {
                process = proc_entry.th32ProcessID;
                CloseHandle(snapshot);
                proc = OpenProcess(PROCESS_ALL_ACCESS, false, process);
                return process;
            }
        } while (Process32NextW(snapshot, &proc_entry));
    }

    CloseHandle(snapshot);
    return 0;
}

HMODULE mem_mgmt::get_mod(uintptr_t pid, const wchar_t* name) {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
    MODULEENTRY32W mod_entry;
    mod_entry.dwSize = sizeof(mod_entry);

    if (Module32FirstW(snapshot, &mod_entry)) {
        do {
            if (!_wcsicmp(mod_entry.szModule, name)) {
                CloseHandle(snapshot);
                return mod_entry.hModule;
            }
        } while (Module32NextW(snapshot, &mod_entry));
    }

    CloseHandle(snapshot);
    return NULL;
}

HMODULE mem_mgmt::get_mod_base(DWORD pid, const std::string& name) {
    MODULEENTRY32 mod_info_pe;
    mod_info_pe.dwSize = sizeof(mod_info_pe);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    if (Module32First(snapshot, &mod_info_pe)) {
        do {
            char module_name[MAX_PATH];
            WideCharToMultiByte(CP_ACP, 0, mod_info_pe.szModule, -1, module_name, MAX_PATH, NULL, NULL);

            if (name == module_name) {
                CloseHandle(snapshot);
                return mod_info_pe.hModule;
            }
        } while (Module32Next(snapshot, &mod_info_pe));
    }

    CloseHandle(snapshot);
    return NULL;
}

