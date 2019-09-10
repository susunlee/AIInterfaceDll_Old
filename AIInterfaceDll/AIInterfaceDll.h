#pragma once

#define AIINTERFACEDLLTYPE __declspec(dllexport)

////////////////////////////////////////////////////////////

struct _NG_FILE_INFO
{
	int				ng_items;
	CString			image_path;
	CStringArray	ai_result;
};

//m_ctlList.InsertColumn(0, _T("번호"), LVCFMT_CENTER, 100);
//m_ctlList.InsertColumn(1, _T("파일명"), LVCFMT_CENTER, r.Width() - 450);
//m_ctlList.InsertColumn(2, _T("NG 갯수"), LVCFMT_CENTER, 150);
//m_ctlList.InsertColumn(3, _T("변환결과"), LVCFMT_CENTER, 150);
//m_ctlList.InsertColumn(4, _T("파일경로"), LVCFMT_CENTER, 0);


CString		m_strPath;
CPtrList	m_listNG;

////////////////////////////////////////////////////////////

extern "C" AIINTERFACEDLLTYPE int GetAIResult(unsigned char *ng_path);

////////////////////////////////////////////////////////////
