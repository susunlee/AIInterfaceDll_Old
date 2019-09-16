#pragma once

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
	int			g_nShowImage;
	CString		m_strPath;
	CPtrList	m_listNG;

} PROC_HANDLE;

PROC_HANDLE		g_hProc;

////////////////////////////////////////////////////////////

extern "C" AIINTERFACEDLLTYPE int GetAIResult(unsigned char *ng_path);

////////////////////////////////////////////////////////////
