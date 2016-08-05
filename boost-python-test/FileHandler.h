#pragma once

#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#include <iostream>
#pragma comment(lib, "User32.lib")

#ifdef UNICODE
#define STDCOUT std::wcout
#define STDSTR  std::wstring
#define SLASH	L"/"
#define ALLFILES	L"/*"
#else
#define STDCOUT std::cout
#define STDSTR  std::string
#define SLASH	"/"
#define ALLFILES	"/*"

#endif

class FileHandler
{
	vector<STDSTR> m_filepath_list;
public:
	int makeListAllFiles(STDSTR rootPath , bool findSubFolder=0) {
		STDSTR	opt_root_path = rootPath; 
		opt_root_path.append(ALLFILES);
		WIN32_FIND_DATA data;
		STDSTR* name;
		HANDLE hFile = FindFirstFile(opt_root_path.c_str(), &data);
		if( hFile == INVALID_HANDLE_VALUE ) {
			return -1;
		} 
		while(FindNextFile(hFile, &data) != 0 || GetLastError() != ERROR_NO_MORE_FILES) {

			if((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && findSubFolder && wcscmp(data.cFileName, L"..")) {
				STDSTR subPath = rootPath;
				subPath.append(SLASH); 
				subPath.append(data.cFileName);
				makeListAllFiles(subPath, true); 
			} else {
				STDSTR absFilePath = rootPath;
				absFilePath.append(SLASH); 
				absFilePath.append(data.cFileName);
				if(!wcscmp(data.cFileName,L".."))
					continue;
				m_filepath_list.push_back(absFilePath);
			}
		}
		return 0;
 	}
	vector<STDSTR> getFilePaths() {
		return m_filepath_list;
	}

	void test(STDSTR rootPath , bool findSubFolder=0)
	{
		makeListAllFiles(rootPath, findSubFolder);
		for(vector<STDSTR>::iterator iter = m_filepath_list.begin(); iter != m_filepath_list.end(); iter++) {
			STDCOUT << *iter << endl;
		}

	}
	


	
};