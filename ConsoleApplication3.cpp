// ConsoleApplication3.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"


char* WcsToMbsUTF8(_In_ const wchar_t* wcs)
{
	_ASSERTE(NULL != wcs);
	if (NULL == wcs) return NULL;

	int outLen = WideCharToMultiByte(CP_UTF8, 0, wcs, -1, NULL, 0, NULL, NULL);
	if (0 == outLen) return NULL;

	char* outChar = (char*)malloc(outLen * sizeof(char));
	if (NULL == outChar) return NULL;
	RtlZeroMemory(outChar, outLen);

	if (0 == WideCharToMultiByte(CP_UTF8, 0, wcs, -1, outChar, outLen, NULL, NULL))
	{
			free(outChar);
		return NULL;
	}

	return outChar;
}



bool is_file_existsW(_In_ const wchar_t* file_path)
{
	_ASSERTE(NULL != file_path);
	_ASSERTE(TRUE != IsBadStringPtrW(file_path, MAX_PATH));
	if ((NULL == file_path) || (TRUE == IsBadStringPtrW(file_path, MAX_PATH))) return false;

	WIN32_FILE_ATTRIBUTE_DATA info = { 0 };

	if (GetFileAttributesExW(file_path, GetFileExInfoStandard, &info) == 0)
		return false;
	else
		return true;
}


/**
* @brief
* @param
* @see
* @remarks
* @code
* @endcode
* @return
**/
void print(_In_ const char* fmt, _In_ ...)
{
	char log_buffer[2048];
	va_list args;

	va_start(args, fmt);
	HRESULT hRes = StringCbVPrintfA(log_buffer, sizeof(log_buffer), fmt, args);
	if (S_OK != hRes)
	{
		fprintf(
			stderr,
			"%s, StringCbVPrintfA() failed. res = 0x%08x",
			__FUNCTION__,
			hRes
			);
		return;
	}

	OutputDebugStringA(log_buffer);
	fprintf(stdout, "%s \n", log_buffer);
}

wchar_t* getCurrentPath(){
	wchar_t *buf = NULL;
	uint32_t buflen = 0;

	buflen = GetCurrentDirectoryW(buflen, buf);
	if (0 == buflen)
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		return NULL;
	}

	buf = (PWSTR)malloc(sizeof(WCHAR) * buflen);
	if (0 == GetCurrentDirectoryW(buflen, buf))
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		free(buf);
		return NULL;
	}
	return buf;
}

/**
* @brief
* @param
* @see
* @remarks
* @code
* @endcode
* @return
**/
bool create_bob_txt()
{
	// current directory 를 구한다.
	wchar_t *buf = NULL;
	uint32_t buflen = 0;
	buflen = GetCurrentDirectoryW(buflen, buf);
	if (0 == buflen)
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		return false;
	}

	buf = (PWSTR)malloc(sizeof(WCHAR) * buflen);
	if (0 == GetCurrentDirectoryW(buflen, buf))
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		free(buf);
		return false;
	}

	// current dir \\ bob.txt 파일명 생성
	wchar_t file_name[260];
	if (!SUCCEEDED(StringCbPrintfW(
		file_name,
		sizeof(file_name),
		L"%ws\\bob.txt",
		buf)))
	{
		print("err, can not create file name");
		free(buf);
		return false;
	}
	free(buf); buf = NULL;

	if (true == is_file_existsW(file_name))
	{
		::DeleteFileW(file_name);
	}

	// 파일 생성
	HANDLE file_handle = CreateFileW(
		file_name,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (file_handle == INVALID_HANDLE_VALUE)
	{
		print("err, CreateFile(path=%ws), gle=0x%08x", file_name, GetLastError());
		return false;
	}


	// 파일에 데이터 쓰기
	DWORD bytes_written = 0;
	wchar_t string_buf[1024];
	if (!SUCCEEDED(StringCbPrintfW(
		string_buf,
		sizeof(string_buf),
		L"동해물과 백두산이 마르고 닳도록 하느님이 보우하사 우리나라만세")))
	{
		print("err, can not create data to write.");
		CloseHandle(file_handle);
		return false;
	}


	//[read1]
	char *strUtf8;

	strUtf8 = WcsToMbsUTF8(string_buf);

	//int nLen = WideCharToMultiByte(CP_UTF8, 0, strUni, lstrlenW(strUni), NULL, 0, NULL, NULL);
	//if(!WideCharToMultiByte (CP_UTF8, 0, string_buf, lstrlenW(string_buf), strUtf8, nLen, NULL, NULL));
	if (!WriteFile(file_handle, strUtf8, strlen(strUtf8), &bytes_written, NULL))
	{
		print("err, WriteFile() failed. gle = 0x%08x", GetLastError());
		CloseHandle(file_handle);
		return false;
	}

	// 영어로 쓰기
	if (!SUCCEEDED(StringCbPrintfW(
		string_buf,
		sizeof(string_buf),
		L"All work and no play makes jack a dull boy.")))
	{
		print("err, can not create data to write.");
		CloseHandle(file_handle);
		return false;
	}

	strUtf8 = WcsToMbsUTF8(string_buf);

	if (!WriteFile(file_handle, strUtf8, strlen(strUtf8), &bytes_written, NULL))
	{
		print("err, WriteFile() failed. gle = 0x%08x", GetLastError());
		CloseHandle(file_handle);
		return false;
	}

	// 파일 닫기
	CloseHandle(file_handle);
	return true;

	buflen = GetCurrentDirectoryW(buflen, buf);
	if (0 == buflen)
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		return false;
	}

	buf = (PWSTR)malloc(sizeof(WCHAR) * buflen);
	if (0 == GetCurrentDirectoryW(buflen, buf))
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		free(buf);
		return false;
	}

	wchar_t currentpath[260];
	wchar_t copypath[260];

	if (!SUCCEEDED(StringCbPrintfW(
		currentpath,
		sizeof(currentpath),
		L"%ws\\bob.txt",
		buf)))
	{
		print("err, can not create file name");
		free(buf);
		return false;
	}

	if (!SUCCEEDED(StringCbPrintfW(
		copypath,
		sizeof(copypath),
		L"%ws\\bob2.txt",
		buf)))
	{
		print("err, can not create file name");
		free(buf);
		return false;
	}

	free(buf); buf = NULL;

	if(!CopyFile(currentpath,copypath, FALSE))
		print("err, can not CopyFile()");

}

bool textCopy(){
	wchar_t *buf = NULL;
	uint32_t buflen = 0;

	buflen = GetCurrentDirectoryW(buflen, buf);
	if (0 == buflen)
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		return false;
	}

	buf = (PWSTR)malloc(sizeof(WCHAR) * buflen);
	if (0 == GetCurrentDirectoryW(buflen, buf))
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		free(buf);
		return false;
	}

	wchar_t currentpath[260];
	wchar_t copypath[260];

	if (!SUCCEEDED(StringCbPrintfW(
		currentpath,
		sizeof(currentpath),
		L"%ws\\bob.txt",
		buf)))
	{
		print("err, can not create file name");
		free(buf);
		return false;
	}

	if (!SUCCEEDED(StringCbPrintfW(
		copypath,
		sizeof(copypath),
		L"%ws\\bob2.txt",
		buf)))
	{
		print("err, can not create file name");
		free(buf);
		return false;
	}

	free(buf); buf = NULL;

	if (!CopyFile(currentpath, copypath, FALSE))
		print("err, can not CopyFile()");
	return true;
}


bool textread()
{
	HANDLE hFile;
	wchar_t *path = NULL;
	char buf[1024] = { 0, };
	DWORD dwRead;
	path = getCurrentPath();
	wchar_t currentpath[1024];
	

	if (!SUCCEEDED(StringCbPrintfW(
		currentpath,
		sizeof(currentpath),
		L"%ws\\bob2.txt",
		path)))
	{
		print("err, can not create file name");
		free(buf);
		return false;
	}

	hFile = CreateFile(currentpath, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		if (ReadFile(hFile, buf, 1024, &dwRead, NULL)){
			CloseHandle(hFile);
		}
		else {
			printf("error ReadFile error\n");
			return FALSE;
		}
		
	}
	else{
		printf("error CreateFIle fail!\n");
		return FALSE;
	}


	wchar_t strUnicode[1024] = { 0, };
	int nLen = MultiByteToWideChar(CP_UTF8, 0, buf, strlen(buf), NULL, NULL);
	MultiByteToWideChar(CP_UTF8, 0, buf, strlen(buf), strUnicode, nLen);

	_cwprintf(L"%s\n", strUnicode);



	return TRUE;
}

bool textread_MMF()
{
	HANDLE hFile;
	wchar_t *path = NULL;
	char buf[1024] = { 0, };
	DWORD dwRead;
	path = getCurrentPath();
	wchar_t currentpath[1024];


	if (!SUCCEEDED(StringCbPrintfW(
		currentpath,
		sizeof(currentpath),
		L"%ws\\bob2.txt",
		path)))
	{
		print("err, can not create file name");
		free(buf);
		return false;
	}

	hFile = CreateFile(currentpath, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		print("err, CreateFile(path=%ws), gle=0x%08x", currentpath, GetLastError());
		return false;
	}

	LARGE_INTEGER fileSize;
	if (TRUE != GetFileSizeEx(hFile, &fileSize))
	{
		print("err, GetFileSizeEx(%ws) failed, gle = %u", currentpath, GetLastError());
		CloseHandle(hFile);
		return false;
	}

	DWORD file_size = (DWORD)fileSize.QuadPart;
	HANDLE file_map = CreateFileMapping(
		hFile,
		NULL,
		PAGE_READONLY,
		0,
		0,
		NULL
		);

	if (NULL == file_map)
	{
		print("err, CreateFileMapping(%ws) failed, gle = %u", currentpath, GetLastError());
		CloseHandle(hFile);
		return false;
	}
	PCHAR file_view = (PCHAR)MapViewOfFile(
		file_map,
		FILE_MAP_READ,
		0,
		0,
		0
		);

	if (file_view == NULL)
	{
		print("err, MapViewOfFile(%ws) failed, gle = %u", currentpath, GetLastError());

		CloseHandle(file_map);
		CloseHandle(hFile);
		return false;
	}


	//UTF8 -> UNICODE
	wchar_t read_unicode2[1024] = { 0, };
	int nLen2 = MultiByteToWideChar(CP_UTF8, 0, file_view, strlen(file_view), NULL, NULL);
	MultiByteToWideChar(CP_UTF8, 0, file_view, strlen(file_view), read_unicode2, nLen2);
	_cwprintf(L"[MMF] %s\n", read_unicode2);

	// close all
	UnmapViewOfFile(file_view);
	CloseHandle(file_map);
	//파일 닫기
	CloseHandle(hFile);
	return true;


}

bool file_delete()
{
	HANDLE hFile;
	wchar_t *path = NULL;
	char buf[1024] = { 0, };
	DWORD dwRead;
	path = getCurrentPath();
	wchar_t file_name[1024];
	wchar_t file_name_copy[1024];


	if (!SUCCEEDED(StringCbPrintfW(
		file_name,
		sizeof(file_name),
		L"%ws\\bob.txt",
		path)))
	{
		print("err, can not create file name");
		free(buf);
		return false;
	}

	if (!SUCCEEDED(StringCbPrintfW(
		file_name_copy,
		sizeof(file_name_copy),
		L"%ws\\bob2.txt",
		path)))
	{
		print("err, can not create file name");
		free(buf);
		return false;
	}
	// 파일 삭제
	DeleteFileW(file_name);
	DeleteFileW(file_name_copy);
}


int _tmain(int argc, _TCHAR* argv[])
{
	create_bob_txt();
	textCopy();
	textread();
	textread_MMF();
	file_delete();
	return 0;
}

