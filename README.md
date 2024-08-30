# Memory-Module
External Memory Module

## code examples

### Obtaining a handle to a process by its name
```cpp
const wchar_t* target_process = L"example_process.exe";
    m.proc = m.get_proc(target_process);
    if (!m.proc) {
        std::wcout << L"Failed to obtain handle for process: " << target_process << std::endl;
        return -1;
    }
    std::wcout << L"Handle obtained for process: " << target_process << std::endl;
```

### Getting the process ID (PID) for the target process
```cpp
    uintptr_t process_id = m.get_pid(target_process);
    if (!process_id) {
        std::wcout << L"Failed to retrieve PID for process: " << target_process << std::endl;
        return -1;
    }
    std::wcout << L"PID for " << target_process << ": " << process_id << std::endl;
```

### Retrieving the base address of a module within the process
```cpp
const wchar_t* target_module = L"example_module.dll";
    HMODULE module_base = m.get_mod(process_id, target_module);
    if (!module_base) {
        std::wcout << L"Failed to get base address for module: " << target_module << std::endl;
        return -1;
    }
    std::wcout << L"Base address for " << target_module << ": " << std::hex << module_base << std::endl;
```

### Reading an integer from a specific address within the module
```cpp
uintptr_t address_to_read = reinterpret_cast<uintptr_t>(module_base) + 0x1234; // Example offset
int read_value = m.read_mem<int>(address_to_read);
std::cout << "Integer read from address " << std::hex << address_to_read << ": " << read_value << std::endl;

```

### Reading a string from memory
```cpp
uintptr_t string_address = reinterpret_cast<uintptr_t>(module_base) + 0x5678; // Example offset
std::string retrieved_string = m.read_string(string_address);
std::cout << "String read from address " << std::hex << string_address << ": " << retrieved_string << std::endl;

```
### Reading a wide string from memory
```cpp
uintptr_t wstring_address = reinterpret_cast<uintptr_t>(module_base) + 0x9ABC; // Example offset
std::wstring retrieved_wstring = m.read_wstring(wstring_address);
std::wcout << L"Wide string read from address " << std::hex << wstring_address << L": " << retrieved_wstring << std::endl;
```
### Close the process handle
```cpp
CloseHandle(m.proc);
```

