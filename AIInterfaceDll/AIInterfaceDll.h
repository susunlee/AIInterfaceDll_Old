#pragma once

#define AIINTERFACEDLLTYPE __declspec(dllexport)

////////////////////////////////////////////////////////////

struct _NG_FILE_INFO
{
	int				ng_items;
	CString			image_path;
	CStringArray	ai_result;
};

//m_ctlList.InsertColumn(0, _T("��ȣ"), LVCFMT_CENTER, 100);
//m_ctlList.InsertColumn(1, _T("���ϸ�"), LVCFMT_CENTER, r.Width() - 450);
//m_ctlList.InsertColumn(2, _T("NG ����"), LVCFMT_CENTER, 150);
//m_ctlList.InsertColumn(3, _T("��ȯ���"), LVCFMT_CENTER, 150);
//m_ctlList.InsertColumn(4, _T("���ϰ��"), LVCFMT_CENTER, 0);


CString		m_strPath;
CPtrList	m_listNG;

////////////////////////////////////////////////////////////

extern "C" AIINTERFACEDLLTYPE int GetAIResult(unsigned char *ng_path);

////////////////////////////////////////////////////////////
