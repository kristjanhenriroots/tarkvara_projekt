#ifndef CCONSOLE_H
#define CCONSOLE_H

#include <QtWidgets/QPlainTextEdit>

class CConsole
{
public:
	CConsole();
	~CConsole();

	void SetPlainTextEdit(QPlainTextEdit *_pPlainTextEdit);
	void Clear(void);
	void Print(const char *_String, ...);

private:
	QPlainTextEdit *pPlainTextEdit;

	void Init(void);
};

typedef CConsole *P_CConsole;

#endif // CCONSOLE_H
