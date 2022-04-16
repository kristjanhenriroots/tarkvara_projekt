#include "cconsole.h"

CConsole::CConsole()
{
	pPlainTextEdit = NULL;
}

CConsole::~CConsole()
{
}

void CConsole::SetPlainTextEdit(QPlainTextEdit *_pPlainTextEdit)
{
	pPlainTextEdit = _pPlainTextEdit; // Uninitialized.
	Init();
}

void CConsole::Clear(void)

{
	if(!pPlainTextEdit) // Uninitialized.
		return;

	pPlainTextEdit->clear();
}

void CConsole::Print(const char *_String, ...)
{
	if(!pPlainTextEdit) // Uninitialized.
		return;

	va_list val;
	char vbuf[1024];

	// Formt text..
	va_start(val, _String);
	vsprintf(vbuf, _String, val);
	va_end(val);

    // Go to the end of content..
	pPlainTextEdit->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);

	// Print..
	pPlainTextEdit->appendHtml(vbuf);
}

void CConsole::Init(void)
{
	QPalette p = pPlainTextEdit->palette();
    p.setColor(QPalette::Base, QColor(255, 255, 255));
	pPlainTextEdit->setPalette(p);
}
