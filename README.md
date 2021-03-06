# Simple-memory-manager
Simple memory manager made in C++/WinAPI (can be used for example in game hacking).

### Functions:
  - CMemory(const std::string ProcessName) [constructor] - opens handle to process via ProcessId by name.
  - CMemory() [constructor] - use that if you dont want to open a handle instantly.
   You need to change m_strProcessName manually before calling another functions.
  
  ***
  
  - GetProcessId() - returns process's id as DWORD.
  - GetProcessHandle() - opens a handle to process.
  - GetProcessBaseAddr() - returns process's base addres as DWORD (base address of first module).
  
  ***
  
  - GetModuleInformation(const std::string ModuleName, ModuleInfo& mInfo) - returns information of module by name.
  
  ```C++
  struct ModuleInfo
  {
	DWORD dwModBaseAddr, dwModSize;
  };
  ```
  
  ***
  
  - Read(DWORD dwAddress) - wrapper to WinAPI's ReadProcessMemory function. 
  - Write(DWORD dwAddress, t val) - wrapper to WinAPI's WriteProcessMemory function.
  
  ***
  
 #### Example usage:
  ```C++
  CMemory memory("process.exe");
  std::cout << " Process id: " << memory.GetProcessId() << std::endl;
  
  ModuleInfo modExample;
  memory.GetModuleInformation("example.dll", modClient);
  ```
  
  ***
  
  ### Error codes:
  Code | Information
  --- | ---
  0 | String m_strProcessName is null.
  1 | Cannot create Process/Module snapshot.
  2 | Cannot find Process by name.
  3 | Cannot find Module by name.
