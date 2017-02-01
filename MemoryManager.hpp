#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

struct ModuleInfo {
	DWORD dwModBaseAddr, dwModSize;
};

class CMemory {
private:
	DWORD m_dwProcessId;
	HANDLE m_hProcess;
public:
	std::string m_strProcessName;

	DWORD GetProcessId()
	{
		if (m_strProcessName == "")
		{
			std::cout << "Error: 0" << std::endl;
			return NULL;
		}

		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

		if (hSnap == INVALID_HANDLE_VALUE)
		{
			std::cout << "Error: 1" << std::endl;
			return NULL;
		}

		PROCESSENTRY32 pEntry;
		pEntry.dwSize = sizeof(PROCESSENTRY32);

		if (Process32First(hSnap, &pEntry))
		{
			do
			{
				if (!strcmp(pEntry.szExeFile, m_strProcessName.c_str()))
				{
					CloseHandle(hSnap);
					m_dwProcessId = pEntry.th32ProcessID;
					return pEntry.th32ProcessID;
				}

			} while (Process32Next(hSnap, &pEntry));
		}

		std::cout << "Error: 2" << std::endl;
		CloseHandle(hSnap);
		return NULL;
	}

	HANDLE GetProcessHandle()
	{
		if (m_strProcessName == "")
		{
			std::cout << "Error: 0" << std::endl;
			return NULL;
		}

		GetProcessId();

		if (m_dwProcessId == NULL)
			return INVALID_HANDLE_VALUE;

		m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_dwProcessId);

		if (m_hProcess == INVALID_HANDLE_VALUE)
			return INVALID_HANDLE_VALUE;

		return m_hProcess;
	}

	DWORD GetProcessBaseAddr()
	{
		if (m_strProcessName == "")
		{
			std::cout << "Error: 0" << std::endl;
			return NULL;
		}

		GetProcessId();

		if (m_dwProcessId == NULL)
			return NULL;

		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_dwProcessId);

		if (hSnap == INVALID_HANDLE_VALUE)
			return NULL;

		MODULEENTRY32 mEntry;
		mEntry.dwSize = sizeof(MODULEENTRY32);

		if (Module32First(hSnap, &mEntry))
		{
			CloseHandle(hSnap);
			return reinterpret_cast<DWORD>(mEntry.modBaseAddr);
		}

		std::cout << "Error 3" << std::endl;
		CloseHandle(hSnap);
		return NULL;
	}

	bool GetModuleInformation(const std::string ModuleName, ModuleInfo& mInfo)
	{
		if (m_strProcessName == "")
		{
			std::cout << "Error: 0" << std::endl;
			return NULL;
		}

		GetProcessId();

		if (m_dwProcessId == NULL)
			return NULL;

		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_dwProcessId);

		if (hSnap == INVALID_HANDLE_VALUE)
			return NULL;

		MODULEENTRY32 mEntry;
		mEntry.dwSize = sizeof(MODULEENTRY32);

		if (Module32First(hSnap, &mEntry))
		{
			do
			{
				if (mEntry.szModule[0] == ModuleName[0] && !strcmp(mEntry.szModule, ModuleName.c_str()))
				{
					mInfo.dwModBaseAddr = reinterpret_cast<DWORD>(mEntry.modBaseAddr);
					mInfo.dwModSize = mEntry.modBaseSize;
					CloseHandle(hSnap);
					return true;
				}

			} while (Module32Next(hSnap, &mEntry));
		}

		std::cout << "Error 3" << std::endl;
		CloseHandle(hSnap);
		return false;
	}

	template <class t>
	t Read(DWORD dwAddress)
	{
		t val;
		ReadProcessMemory(m_hProcess, (LPVOID)dwAddress, &val, sizeof(t), NULL);
		return val;
	}

	template <class t>
	void Write(DWORD dwAddress, t val)
	{
		WriteProcessMemory(m_hProcess, (LPVOID)dwAddress, &val, sizeof(t), NULL);
	}

	CMemory()
	{
		m_strProcessName = "";
	}

	CMemory(const std::string ProcessName)
	{
		m_dwProcessId = NULL;
		m_hProcess = INVALID_HANDLE_VALUE;
		m_strProcessName = ProcessName;

		GetProcessHandle();
	}
};
