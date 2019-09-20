#pragma once


#define AIINTERFACEDLLTYPE		__declspec(dllexport)

#define	NUM_OF_DIVISION			5
#define MAX_CSV_PROC			(MAXIMUM_WAIT_OBJECTS - 1)

////////////////////////////////////////////////////////////

struct _NG_FILE_INFO
{
	int				parent_id;
	int				ng_items;
	CString			image_path;
	CStringArray	ai_result;
};


typedef struct _PROC_HANDLE
{
	CString			ng_path;
	CPtrList		ng_list;

} PROC_HANDLE;

int					g_nShowImage;
PROC_HANDLE			g_hProc[NUM_OF_DIVISION];


////////////////////////////////////////////////////////////

extern "C" AIINTERFACEDLLTYPE int GetAIResult(unsigned char *ng_path);

////////////////////////////////////////////////////////////
