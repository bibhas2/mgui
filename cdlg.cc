#include "mgui.h"
#include <iostream>
#include <commdlg.h>
CFileDialog::CFileDialog(const char* title, const char* filter, CWindow* parent) {
	::ZeroMemory(&of, sizeof(of));
	fileName[0] = '\0';

	of.lStructSize = sizeof(of);
	if (parent != NULL) {
		of.hwndOwner = parent->m_wnd;
	}
	of.lpstrFile = fileName;
	of.nMaxFile = sizeof(fileName) - 1;
	of.lpstrTitle = title;
	of.Flags = OFN_OVERWRITEPROMPT;

	/*
	 * Filter string convention:
	 * "Text file|*.txt;*log|Image files|*.gif;*.jpg"
	 * This convention is not from MS. MS expects the
	 * seperator to be '\0' instead of |.
	 */
	int i;

	for (i = 0; 
		filter != NULL && 
		filter[i] != '\0' &&
		i < sizeof(filterBuff) - 2; 
		++i) {
		if (filter[i] == '|') {
			filterBuff[i] = '\0';
		} else {
			filterBuff[i] = filter[i];
		}
	}
	filterBuff[i] = '\0'; ++i;
	filterBuff[i] = '\0';
	of.lpstrFilter = filterBuff;
}

bool CFileDialog::openFile() {
	BOOL res = ::GetOpenFileName(&of);

	return res == TRUE;
}

bool CFileDialog::saveFile() {
	BOOL res = ::GetSaveFileName(&of);

	return res == TRUE;
}

const char* CFileDialog::getFileName() {
	return fileName;
}
