#include "KernelCore.h"

BOOL SetPrivilege(LPCTSTR Privilege, BOOL bEnablePrivilege)
{
	HANDLE token_privilege;
	TOKEN_PRIVILEGES tp;
	LUID luid;
	TOKEN_PRIVILEGES tpPrevious;
	DWORD cbPrevious = sizeof(TOKEN_PRIVILEGES);

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token_privilege))
	{
		if (!ImpersonateSelf(SecurityImpersonation) || !OpenProcessToken(GetCurrentProcess(),
		                                                                 TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
		                                                                 &token_privilege))
			return FALSE;
	}

	if (!LookupPrivilegeValue(NULL, Privilege, &luid))
		return FALSE;

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = 0;

	AdjustTokenPrivileges(token_privilege, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), &tpPrevious, &cbPrevious);

	if (GetLastError() != ERROR_SUCCESS)
		return FALSE;

	tpPrevious.PrivilegeCount = 1;
	tpPrevious.Privileges[0].Luid = luid;

	if (bEnablePrivilege)
		tpPrevious.Privileges[0].Attributes |= (SE_PRIVILEGE_ENABLED);
	else
		tpPrevious.Privileges[0].Attributes ^= (SE_PRIVILEGE_ENABLED & tpPrevious.Privileges[0].Attributes);

	AdjustTokenPrivileges(token_privilege, FALSE, &tpPrevious, cbPrevious, NULL, NULL);

	return GetLastError() == ERROR_SUCCESS;
}

bool RC_CallConv AttachDebuggerToProcess(RC_Pointer) { return false; }
void RC_CallConv DetachDebuggerFromProcess(RC_Pointer){}
bool RC_CallConv AwaitDebugEvent(DebugEvent*, int) { return false; }
void RC_CallConv HandleDebugEvent(DebugEvent*){}
bool RC_CallConv SetHardwareBreakpoint(RC_Pointer, RC_Pointer, HardwareBreakpointRegister,
                                       HardwareBreakpointTrigger, HardwareBreakpointSize, bool) { return false; }
