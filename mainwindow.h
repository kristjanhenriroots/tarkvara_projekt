#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QMainWindow>
#include <QMessageBox>
#include <QCheckBox> // QCheckBox support.
#include <QRadioButton> // QRadioButton support.
#include <QFileDialog> // QFileDialog support.
#include <QImageReader>
#include <QColorDialog>
#include "capp.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

 //-------------- Main window class --------------------------------

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private:
	Ui::MainWindow *ui;
	P_CConsole pConsole;
	COLOR_RGB Colors[element_n]; // Elements colors.
	P_CApp pApp;

	// UI signal procedures..
	void btnClrWallOnPressed(void); // Color wall 'Pressed'
	void btnClrPathOnPressed(void); // Color path 'Pressed'
	void btnClrRecPathOnPressed(void); // Color recursive path 'Pressed'
	void btnClrBfsPathOnPressed(void); // Color BFS path 'Pressed'
	void btnClrCrosOnPressed(void); // Color crossover 'Pressed'
	void btnClrRstOnPressed(void); // Color reset 'Pressed'
	void btnAlg1OnPressed(void); // Algo1 'Pressed'.
	void btnAlg2OnPressed(void); // Algo2 'Pressed'.
	void btnAlg3OnPressed(void); // Algo3 'Pressed'.
	void spnSizOnChanged(int _Val); // Size 'Ghanged'.
	void btnGenOnPressed(void); // Generate 'Pressed'.
	void btnGenSaveOnPressed(void); // Save generated 'Pressed'.
	void btnGenLoadOnPressed(void); // Load generated 'Pressed'.
	void btnSolvOnPressed(void); // Solve 'Pressed'.
	void btnSolvSaveOnPressed(void); // Save solved 'Pressed'.
	// UI window events..
	void resizeEvent(QResizeEvent *_pQResizeEvent); // Window size change.
	// Functions..
	void ClrRst(void); // Reset colors.
	int ViewImage(const char *_Filename); // Display image file.
    void ChangeDisplayedColor(void); //auris added
};

#endif // _MAINWINDOW_H_



//-----------
