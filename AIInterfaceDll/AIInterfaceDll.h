#pragma once

#include <afxmt.h>

#define AIINTERFACEDLLTYPE __declspec(dllexport)

////////////////////////////////////////////////////////////

struct _NG_FILE_INFO
{
	int				ng_items;
	CString			image_path;
	CStringArray	ai_result;
};


typedef struct _PROC_HANDLE
{
	int				show_image;
	CString			ng_path;
	CPtrList		ng_list;

} PROC_HANDLE;

PROC_HANDLE			g_hProc;
CCriticalSection	g_hCS;

////////////////////////////////////////////////////////////

extern "C" AIINTERFACEDLLTYPE int GetAIResult(unsigned char *ng_path);

////////////////////////////////////////////////////////////
