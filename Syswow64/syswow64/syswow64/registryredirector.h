#pragma once
#include "tchar.h"

#if defined(_WIN64)

#define VIEW_DATA L"Hello! 64-bit World"
#define ALT_VIEW_DATA L"Hello! 32-bit World"
#define CROSS_ACCESS KEY_WOW64_32KEY

#else

#define VIEW_DATA L"Hello! 32-bit World"
#define ALT_VIEW_DATA L"Hello! 64-bit World"
#define CROSS_ACCESS KEY_WOW64_64KEY

#endif

// Create a registry key and set its value.
// Return TRUE if the function succeeds, FALSE if it fails.
BOOL
CreateRegistryKeyValue(
  HKEY hKeyParent,
  LPCWSTR KeyName,
  REGSAM rsAccessMask,
  REGSAM rsSamViewFlag,
  PBYTE pValue,
  DWORD dwValueSize
)
{
  DWORD dwDisposition;
  HKEY  hKey;
  DWORD dwRet;

  dwRet =
    RegCreateKeyEx(
      hKeyParent,
      KeyName,
      0,
      NULL,
      REG_OPTION_NON_VOLATILE,
      rsAccessMask | rsSamViewFlag,
      NULL,
      &hKey,
      &dwDisposition);

  if (dwRet != ERROR_SUCCESS)
  {
    printf("Error opening or creating key.\n");
    return FALSE;
  }

  // Attempt to set the value of the key.
  // If the call fails, close the key and return.
  if (ERROR_SUCCESS !=
    RegSetValueEx(
      hKey,
      NULL,
      0,
      REG_SZ,
      pValue,
      dwValueSize))
  {
    printf("Could not set registry value.\n");
    RegCloseKey(hKey);
    return FALSE;
  }

  RegCloseKey(hKey);
  return TRUE;
}

// Access a registry key and print its value.
// Return TRUE if the function succeeds, FALSE if it fails.
BOOL
AccessRegistryKeyValue(
  HKEY hKeyParent,
  LPCWSTR KeyName,
  REGSAM rsAccessMask,
  REGSAM rsSamViewFlag
)
{
  HKEY hKey;
  WCHAR Buffer[MAX_PATH];
  DWORD dwSize = sizeof(Buffer);
  DWORD dwType;
  DWORD dwRet;

  dwRet =
    RegOpenKeyEx(
      hKeyParent,
      KeyName,
      0,
      rsAccessMask | rsSamViewFlag,
      &hKey);

  if (dwRet != ERROR_SUCCESS)
  {
    printf("Error opening key!\n");
    return FALSE;
  }

  if (ERROR_SUCCESS !=
    RegQueryValueEx(
      hKey,
      NULL,
      0,
      &dwType,
      (PBYTE)Buffer,
      &dwSize))
  {
    printf("Could not read registry value.\n");
    RegCloseKey(hKey);
    return FALSE;
  }

  if (rsSamViewFlag != 0)
  {
    printf("Alternate view:   %S\n", Buffer);
  }
  else
  {
    printf("Default view:     %S\n", Buffer);
  }

  RegCloseKey(hKey);
  return TRUE;
}

void TestRegistryRedirector() {
  CreateRegistryKeyValue(HKEY_LOCAL_MACHINE, _T("Software\\Hello World"), KEY_READ | KEY_WRITE,
    0,
    (PBYTE)VIEW_DATA,
    sizeof(VIEW_DATA));

  AccessRegistryKeyValue(HKEY_LOCAL_MACHINE, _T("Software\\Hello World"), KEY_READ, 0);

  AccessRegistryKeyValue(HKEY_LOCAL_MACHINE, _T("Software\\Hello World"), KEY_READ, CROSS_ACCESS);
}
