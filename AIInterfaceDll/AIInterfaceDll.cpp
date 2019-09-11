// AIInterfaceDll.cpp : DLL의 초기화 루틴을 정의합니다.
//

#include "stdafx.h"
#include "AIInterfaceDll.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DLLEXPORT

#define	SHOW_IMAGE					1

int GetNGCount(CString f_name)
{
	int items = 0;
	CString temp;
	CStdioFile file(f_name, CFile::modeRead | CFile::typeText);

	while (1)
	{
		if (file.ReadString(temp))
		{
			items++;
		}
		else
		{
			break;
		}
	}

	file.Close();

	return items - 11;		// csv 파일의 헤더(?) 11 라인 이후부터 NG 정보 (라인 하나가 하나의 NG에 대한 정보임)
}

void DrawRawDataIntoDC(CDC* pDC, BYTE* pViewImg, int x, int y, int width, int height, int bitCount)
{
	BITMAPINFO *pBmpInfo = NULL;
	if (pBmpInfo != NULL)
	{
		delete[] pBmpInfo;
		pBmpInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFOHEADER) + (256 * sizeof(RGBQUAD))];
	}
	else
		pBmpInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFOHEADER) + (256 * sizeof(RGBQUAD))];


	pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pBmpInfo->bmiHeader.biPlanes = 1;
	pBmpInfo->bmiHeader.biCompression = 0; //BI_RGB;// 0;  
	pBmpInfo->bmiHeader.biWidth = width; //이미지 가로 크기를 적음.  
	pBmpInfo->bmiHeader.biHeight = height;
	pBmpInfo->bmiHeader.biBitCount = bitCount;
	pBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	pBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	pBmpInfo->bmiHeader.biClrUsed = 256;
	pBmpInfo->bmiHeader.biClrImportant = 0;//SHRT_MAX;  
	pBmpInfo->bmiHeader.biSizeImage = height * WIDTHBYTES(width * bitCount);

	if (bitCount == 8)
	{
		for (int i = 0; i < 256; i++)
		{
			pBmpInfo->bmiColors[i].rgbBlue = i;
			pBmpInfo->bmiColors[i].rgbGreen = i;
			pBmpInfo->bmiColors[i].rgbRed = i;
			pBmpInfo->bmiColors[i].rgbReserved = 0;
		}
	}


	//pViewImg 가 RAW DATA
	if (pViewImg != NULL)
	{
		//화면에 출력하기 전에 메모리에 먼저 적재한다. 또한, 비트맵 구조체를 만든다.  
		CRect rect;
		CDC memDC;
		CBitmap bitMap, *pOldBitmap;

		// 호환되는 memDC를 메모리에 생성한다.  
		memDC.CreateCompatibleDC(pDC);
		bitMap.CreateCompatibleBitmap(pDC, width, height);
		pOldBitmap = (CBitmap*)memDC.SelectObject(&bitMap);
		memDC.SelectObject(&bitMap);
		memDC.FillSolidRect(&rect, RGB(255, 255, 255));

		// image 역상

		int rwsize = WIDTHBYTES(width * bitCount);
		for (int i = 0; i < height / 2; i++) {
			for (int j = 0; j < width * 3; j++) {
				char temp = pViewImg[i*rwsize + j];
				pViewImg[i*rwsize + j] = pViewImg[(height - i - 1)* rwsize + j];
				pViewImg[(height - i - 1)* rwsize + j] = temp;
			}
		}


		SetDIBitsToDevice(
			memDC.GetSafeHdc(), // Handle to the device context.  
			0, 0, // Specifies the x and y-coordinate, in logical units, of the upper-left corner of the destination rectangle.  
			width, height, //가로, 세로를 적는다.  
			// Specifies the width and height, in logical units, of the DIB.  
			0, 0, // Specifies the x and y-coordinate, in logical units, of the lower-left corner of the DIB.  
			0, // Specifies the starting scan line in the DIB.  
			height, //세로를 적음. // Specifies the number of DIB scan lines contained in the array pointed to by the lpvBits parameter.  
			pViewImg, pBmpInfo, DIB_RGB_COLORS);



		//StretchBltMode 설정.  
		pDC->SetStretchBltMode(COLORONCOLOR);

		//dc.BitBlt(0, 0, rectLP.right, rectLP.bottom, &memDC,0, 0, SRCCOPY); 


		//pDC->StretchBlt(0,0,Width, Height, &memDC, 0, 0, Width, Height, SRCCOPY);   //크기조절 가능하다.
		pDC->BitBlt(x, y, width, height, &memDC, 0, 0, SRCCOPY);

	}


	if (pBmpInfo != NULL)
	{
		delete[] pBmpInfo;
		pBmpInfo = NULL;
	}
}

int ProcAI(_NG_FILE_INFO *ni, CString src_csv, CString target_scan, CString target_master)
{
	CImage image_scan, image_master;
	CFileStatus fs_scan, fs_master;
	CString path_scan = ni->image_path;
	CString path_master = ni->image_path;

	path_scan.Replace(src_csv, target_scan);
	path_master.Replace(src_csv, target_master);

	if ((ni->ng_items < 1) || 
		(CFile::GetStatus(path_scan, fs_scan) == FALSE) || 
		(CFile::GetStatus(path_master, fs_master) == FALSE))
	{
		return 0;
	}

	image_scan.Load(path_scan);
	image_master.Load(path_master);

	CString msg;
	BYTE* tmp_scan = NULL;
	BYTE* tmp_master = NULL;
	BYTE* src_scan_save = NULL;
	BYTE* src_master_save = NULL;
	BYTE* src_scan = NULL;
	BYTE* src_master = NULL;
	BYTE* src_scan1 = (BYTE*)image_scan.GetBits();
	BYTE* src_master1 = (BYTE*)image_master.GetBits();
	int p_scan = image_scan.GetPitch();
	int h_scan = image_scan.GetHeight();
	int p_master = image_master.GetPitch();
	int h_master = image_master.GetHeight();

	//////////////////////////////////////////////////////////////////////////
	if (p_scan < 0)
	{
		src_scan1 += (p_scan * (h_scan - 1));
	}

	src_scan = (BYTE*)malloc(abs(p_scan) * h_scan);
	src_scan_save = src_scan;

	for (int i = (h_scan - 1); -1 < i; i--)
	{
		memcpy(src_scan + (i * abs(p_scan)), src_scan1, abs(p_scan));

		src_scan1 += abs(p_scan);
	}

	tmp_scan = (BYTE*)malloc(IMAGE_WIDTH * 3 * IMAGE_HEIGHT);

	////////////////////////////////////////////////////////////////////////
	if (p_master < 0)
	{
		src_master1 += (p_master * (h_master - 1));
	}

	src_master = (BYTE*)malloc(abs(p_master) * h_master);
	src_master_save = src_master;

	for (int i = (h_master - 1); -1 < i; i--)
	{
		memcpy(src_master + (i * abs(p_master)), src_master1, abs(p_master));

		src_master1 += abs(p_master);
	}

	tmp_master = (BYTE*)malloc(IMAGE_WIDTH * 3 * IMAGE_HEIGHT);

	/////////////////////////////////////////////////////////////////////////

	for (int i = 0, k = 0; i < ni->ng_items; i++, k++)
	{
		for (int j = 0; j < IMAGE_HEIGHT; j++)
		{
			memcpy(tmp_scan + (j * (IMAGE_WIDTH * 3)), src_scan + (j * abs(p_scan)) + ((k % IMAGE_ITEMS_PER_LINE) * IMAGE_WIDTH * 3), IMAGE_WIDTH * 3);
		}

		for (int j = 0; j < IMAGE_HEIGHT; j++)
		{
			memcpy(tmp_master + (j * (IMAGE_WIDTH * 3)), src_master + (j * abs(p_master)) + ((k % IMAGE_ITEMS_PER_LINE) * IMAGE_WIDTH * 3), IMAGE_WIDTH * 3);
		}

		//////////////////////////////////////////////////////////////////////
		//
		//           AI 함수 호출 후 결과 저장
		msg.Format(_T("%d"), Sum(i + 1, 0));
		ni->ai_result.Add(msg);
		//
		//////////////////////////////////////////////////////////////////////

		if (SHOW_IMAGE)
		{
			CString temp_path = path_scan;

			msg.Format(_T("defect_image\\[%05d] "), i + 1);
			temp_path.Insert(g_hProc.m_strPath.GetLength() + 1, msg);
			temp_path.Replace(_T(".jpg"), _T(".raw"));

			CT2CA szPath(temp_path, CP_ACP);
			FILE *fp = NULL;
			fopen_s(&fp, szPath, "wb");
			fwrite(tmp_scan, 1, (IMAGE_WIDTH * 3 * IMAGE_HEIGHT), fp);
			fclose(fp);

			CDC *hDC = CDC::FromHandle(GetWindowDC(GetDesktopWindow()));
			DrawRawDataIntoDC(hDC, tmp_scan, (i % IMAGE_ITEMS_PER_LINE) * IMAGE_WIDTH, (i / IMAGE_ITEMS_PER_LINE) * IMAGE_HEIGHT, IMAGE_WIDTH, IMAGE_HEIGHT, image_scan.GetBPP());
			hDC->DeleteDC();
		}

		if ((i > 0) && ((i % IMAGE_ITEMS_PER_LINE) == 9))
		{
			src_scan += (abs(p_scan) * IMAGE_WIDTH);
			src_master += (abs(p_master) * IMAGE_WIDTH);
		}
	}

	free(tmp_scan);
	free(src_scan_save);

	free(tmp_master);
	free(src_master_save);

	return 1;
}


void UpdateCSV(_NG_FILE_INFO *ni)
{
	int items = 0, ai_result_index = 0;
	CString temp;
	CFileStatus fs_old;
	temp.Format(_T("%s.old"), ni->image_path);
	if (CFile::GetStatus(temp, fs_old) == TRUE)
	{
		CFile::Remove(temp, NULL);
	}

	CFile::Rename(ni->image_path, temp);

	CStdioFile file_old(temp, CFile::modeRead | CFile::typeText);
	CStdioFile file_new(ni->image_path, CFile::modeCreate | CFile::modeWrite | CFile::typeText);

	while (1)
	{
		if (file_old.ReadString(temp))
		{
			if (10 < items)
			{
				int index = temp.ReverseFind(_T(','));

				temp = temp.Left(index + 1);

				temp += ni->ai_result.GetAt(ai_result_index++);
			}

			temp += _T("\n");

			file_new.WriteString(temp);

			items++;
		}
		else
		{
			break;
		}
	}

	file_old.Close();
	file_new.Close();
}


///////////////////////////////////////////////////////////////////////////////////

DWORD CALLBACK ProcThread(LPVOID pParam)
{
	PROC_HANDLE *ph = (PROC_HANDLE*)pParam;

	CFileFind ff;
	CString csv_path, temp;
	BOOL is_ok = FALSE;
	int ng_count = 0;
	int ret = 1;

	for (POSITION pos = g_hProc.m_listNG.GetHeadPosition(); pos != NULL;)
	{
		_NG_FILE_INFO *p = (_NG_FILE_INFO*)ph->m_listNG.GetNext(pos);

		delete p;
	}

	g_hProc.m_listNG.RemoveAll();


	csv_path.Format(_T("%s\\*.csv"), ph->m_strPath);
	is_ok = ff.FindFile(csv_path);


	while (is_ok)
	{
		is_ok = ff.FindNextFile();

		if (ff.IsArchived())
		{
			_NG_FILE_INFO *ni = new _NG_FILE_INFO;

			ni->ng_items = GetNGCount(ff.GetFilePath());
			ni->image_path = ff.GetFilePath();

			ph->m_listNG.AddTail(ni);

			ng_count += ni->ng_items;
		}
	}

	if (0 < ng_count)
	{
		if (SHOW_IMAGE)
		{
			temp.Format(_T("%s\\defect_image"), ph->m_strPath);
			CreateDirectory(temp, NULL);
			temp.Format(_T("%s\\master_image"), ph->m_strPath);
			CreateDirectory(temp, NULL);
		}

		for (POSITION pos = ph->m_listNG.GetHeadPosition(); pos != NULL;)
		{
			_NG_FILE_INFO *ni = (_NG_FILE_INFO*)ph->m_listNG.GetNext(pos);

			if (ProcAI(ni, _T(".csv"), _T("_defect.jpg"), _T("_master.jpg")) == 1)
			{
				UpdateCSV(ni);
			}
			else
			{
				ret = 0;
			}
		}

		for (POSITION pos = ph->m_listNG.GetHeadPosition(); pos != NULL;)
		{
			_NG_FILE_INFO *p = (_NG_FILE_INFO*)ph->m_listNG.GetNext(pos);

			delete p;
		}

		ph->m_listNG.RemoveAll();
	}


	return 1;
}


///////////////////////////////////////////////////////////////////////////////////

extern "C" AIINTERFACEDLLTYPE int GetAIResult(unsigned char *ng_path)
{
	g_hProc.m_strPath = CA2CT((char*)ng_path);

	if (g_hProc.m_strPath.GetAt(g_hProc.m_strPath.GetLength() - 1) == _T('\\'))
	{
		g_hProc.m_strPath.Delete(g_hProc.m_strPath.GetLength() - 1);
	}

	HANDLE h = CreateThread(NULL, 0, ProcThread, &g_hProc, 0, NULL);

	DWORD dw = WaitForSingleObject(h, INFINITE); //
	if (dw == WAIT_FAILED)
	{
		//puts("thread error");
		//return 0;
	}


	return 1;
}
