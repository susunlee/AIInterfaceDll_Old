// 리턴값 : -1 에러(처리를 못함)
//        :  0 NG이미지
//		  :  1 OK이미지
int AI_FOR_VERIFY_201909(unsigned char *master_raw,    // master raw 포인터(200*600)
						 unsigned char *defect_raw);   // defect raw 포인터(200*600)