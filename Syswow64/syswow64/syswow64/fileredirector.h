#pragma once
#include <iostream>
#include <windows.h>
#include <strsafe.h>
#include <tchar.h>


void X86AccessSystem32Files()
{
  //with x86 c:\\windows\\system32\\syswow64filepathtest
  CreateFile(_T("c:\\windows\\Sysnative\\syswow64filepathtest"), GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
}

void AccessSystemFiles()
{
  TCHAR path[MAX_PATH];
  GetSystemDirectory(path, MAX_PATH); //c:\\windows\\system32
  StringCbCat(path, MAX_PATH, _T("\\syswow64filepathtest")); // c:\\windows\\system32\\syswow64filepathtest

  //with x64 c:\\windows\\system32\\syswow64filepathtest
  //with x86 c:\\windows\\SysWow64\\syswow64filepathtest
  CreateFile(path, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
}

void ProgramFilesEnvironment()
{
  //with x64 C:\\Program Files
  //with x86 C:\\Program Files (x86)
  TCHAR path[32767] = { 0 };
  GetEnvironmentVariable(_T("PROGRAMFILES"), path, 32767);
}

void TestFileDirector() {
  //AccessSystemFiles();
  //X86AccessSystem32Files();
  //ProgramFilesEnvironment();
}