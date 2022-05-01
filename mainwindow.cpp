#include "mainwindow.h"
#include "ui_mainwindow.h"

// Public


// Constructor..
MainWindow::MainWindow(QWidget *parent):QMainWindow(parent), ui(new Ui::MainWindow)
{
	char Txt[512];

	// Setup UI..
	ui->setupUi(this);

	// Console init..
	pConsole = new CConsole;
	pConsole->SetPlainTextEdit(ui->edtConsole);

	// Connect UI signals..
	connect(ui->btnClrWall, &QPushButton::pressed, this, &MainWindow::btnClrWallOnPressed); // Color walls 'Pressed'.
	connect(ui->btnClrPath, &QPushButton::pressed, this, &MainWindow::btnClrPathOnPressed); // Color path 'Pressed'.
	connect(ui->btnClrRecPath, &QPushButton::pressed, this, &MainWindow::btnClrRecPathOnPressed); // Color recursive path 'Pressed'.
	connect(ui->btnClrBfsPath, &QPushButton::pressed, this, &MainWindow::btnClrBfsPathOnPressed); // Color bfs path 'Pressed'.
	connect(ui->btnClrCros, &QPushButton::pressed, this, &MainWindow::btnClrCrosOnPressed); // Color crosover 'Pressed'.
	connect(ui->btnClrRst, &QPushButton::pressed, this, &MainWindow::btnClrRstOnPressed); // Color reset 'Pressed'.
	connect(ui->radAlgo1, &QPushButton::pressed, this, &MainWindow::btnAlg1OnPressed); // Algo1 'Pressed'.
	connect(ui->radAlgo2, &QPushButton::pressed, this, &MainWindow::btnAlg2OnPressed); // Algo2 'Pressed'.
	connect(ui->radAlgo3, &QPushButton::pressed, this, &MainWindow::btnAlg3OnPressed); // Algo3 'Pressed'.
	connect(ui->spnSiz, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::spnSizOnChanged); // Maze size 'Changed'.
	connect(ui->btnGen, &QPushButton::pressed, this, &MainWindow::btnGenOnPressed); // Generate 'Pressed'.
	connect(ui->btnGenSave, &QPushButton::pressed, this, &MainWindow::btnGenSaveOnPressed); // Save generate 'Pressed'.
	connect(ui->btnGenLoad, &QPushButton::pressed, this, &MainWindow::btnGenLoadOnPressed); // Load generate 'Pressed'.
	connect(ui->btnSolv, &QPushButton::pressed, this, &MainWindow::btnSolvOnPressed); // Solve 'Pressed'.
	connect(ui->btnSolvSave, &QPushButton::pressed, this, &MainWindow::btnSolvSaveOnPressed); // Save solved 'Pressed'.

	// Setup spines..
	ui->spnSiz->setMinimum(MAZEMIN); // Maze size: Min.
	ui->spnSiz->setMaximum(MAZEMAX); // Maze size: Max.

	// Setup display image area..
	ui->lblImg->setScaledContents(true);

	// Application ID..
	sprintf(Txt, "%s [%d.%d.%d]\n", APP_NAME, APP_VER_MAJOR, APP_VER_MINOR, APP_VER_BUILD);
	setWindowTitle(Txt);
	pConsole->Print("%s\n", Txt);
	pConsole->Print("\n");

	// Color reset..
	ClrRst();

	// Application init..
	pApp = new CApp; // Initiate app.
	pApp->SetConsole(pConsole);
}

// Destructor..
MainWindow::~MainWindow()
{
	if(pApp) // Delete application class.
	{
		delete pApp;
		pApp = nullptr;
	}
	delete ui;
}

//privatee ----------------------

/*
 * UI signal procedures.
 */

// Color wall 'Pressed'
void MainWindow::btnClrWallOnPressed(void)
{
	QColor color, colorNew;

	color.setRgb(Colors[wall].Red, Colors[wall].Green, Colors[wall].Blue);

	colorNew = QColorDialog::getColor(color, parentWidget());
	if(!colorNew.isValid())
		return;

	Colors[wall].Red = colorNew.red();
	Colors[wall].Green = colorNew.green();
	Colors[wall].Blue = colorNew.blue();

	color.setRgb(Colors[wall].Red, Colors[wall].Green, Colors[wall].Blue);
	ui->btnClrWall->setStyleSheet("background-color: " + color.name());
	ChangeDisplayedColor();
}

// Color path 'Pressed'.
void MainWindow::btnClrPathOnPressed(void)
{
	QColor color, colorNew;

	color.setRgb(Colors[path].Red, Colors[path].Green, Colors[path].Blue);

	colorNew = QColorDialog::getColor(color, parentWidget());
	if(!colorNew.isValid())
		return;

	Colors[path].Red = colorNew.red();
	Colors[path].Green = colorNew.green();
	Colors[path].Blue = colorNew.blue();

	color.setRgb(Colors[path].Red, Colors[path].Green, Colors[path].Blue);
	ui->btnClrPath->setStyleSheet("background-color: " + color.name());
	ChangeDisplayedColor();
}

// Color recursive path 'Pressed'.
void MainWindow::btnClrRecPathOnPressed(void)
{
	QColor color, colorNew;

	color.setRgb(Colors[rec_path].Red, Colors[rec_path].Green, Colors[rec_path].Blue);

	colorNew = QColorDialog::getColor(color, parentWidget());
	if(!colorNew.isValid())
		return;

	Colors[rec_path].Red = colorNew.red();
	Colors[rec_path].Green = colorNew.green();
	Colors[rec_path].Blue = colorNew.blue();

	color.setRgb(Colors[rec_path].Red, Colors[rec_path].Green, Colors[rec_path].Blue);
	ui->btnClrRecPath->setStyleSheet("background-color: " + color.name());
	ChangeDisplayedColor();
}

// Color BFS path 'Pressed'.
void MainWindow::btnClrBfsPathOnPressed(void)
{
	QColor color, colorNew;

	color.setRgb(Colors[bfs_path].Red, Colors[bfs_path].Green, Colors[bfs_path].Blue);

	colorNew = QColorDialog::getColor(color, parentWidget());
	if(!colorNew.isValid())
		return;

	Colors[bfs_path].Red = colorNew.red();
	Colors[bfs_path].Green = colorNew.green();
	Colors[bfs_path].Blue = colorNew.blue();

	color.setRgb(Colors[bfs_path].Red, Colors[bfs_path].Green, Colors[bfs_path].Blue);
	ui->btnClrBfsPath->setStyleSheet("background-color: " + color.name());
	ChangeDisplayedColor();
}

// Color crossover 'Pressed'.
void MainWindow::btnClrCrosOnPressed(void)
{
	QColor color, colorNew;

	color.setRgb(Colors[crossover].Red, Colors[crossover].Green, Colors[crossover].Blue);

	colorNew = QColorDialog::getColor(color, parentWidget());
	if(!colorNew.isValid())
		return;

	Colors[crossover].Red = colorNew.red();
	Colors[crossover].Green = colorNew.green();
	Colors[crossover].Blue = colorNew.blue();

	color.setRgb(Colors[crossover].Red, Colors[crossover].Green, Colors[crossover].Blue);
	ui->btnClrCros->setStyleSheet("background-color: " + color.name());
	ChangeDisplayedColor();
}

// Color reset 'Pressed'
void MainWindow::btnClrRstOnPressed(void)
{
	ClrRst();
		//ui->radFadeN->setChecked(true);
	ui->chkClrIns->setChecked(false);
		ChangeDisplayedColor();
}

// Algo1 'Pressed'.
void MainWindow::btnAlg1OnPressed(void)
{
	// Disable 'Loops' parameter..
	ui->chkLops->setEnabled(false);
}

// Algo2 'Pressed'.
void MainWindow::btnAlg2OnPressed(void)
{
	// Enable 'Loops' parameter..
	ui->chkLops->setEnabled(true);
}

// Algo3 'Pressed'.
void MainWindow::btnAlg3OnPressed(void)
{
	// Disable 'Loops' parameter..
	ui->chkLops->setEnabled(false);
}

// Size 'Ghanged'.
void MainWindow::spnSizOnChanged(int _Val)
{
	// Maze size must be an odd number!!..

	// Automatic on the fly 'even' to 'odd' conversion,
    // This may interfere with direct manual input!.. uhhhhhhhh +++++++++++++++++++++++++++
	if((_Val % 2) == 0) // Even!
	{
		_Val++; // Odd it.
		ui->spnSiz->setValue(_Val); // Set it.
	}
}

// Generate 'Pressed'.
void MainWindow::btnGenOnPressed(void)
{
	int algo, lops, siz, mode;

	algo = null;
	lops = 0;

	if(ui->radAlgo1->isChecked()) // Eller's.
	{
		algo = eller;
	}

	if(ui->radAlgo2->isChecked()) // Growing tree backtracer.
	{
		algo = tree;
		if(ui->chkLops->isChecked()) // Loops..
		{
			lops = 2; // Set loops.
		}
		else // No loops.
		{
			lops = 0; // Set loops.
		}
	}

	if(ui->radAlgo3->isChecked()) // Growing tree Prim.
	{
		algo = prim;
	}

	siz = ui->spnSiz->text().toInt(); // Get size.

	mode = secret;
	if(ui->chkClrIns->isChecked())
	{
		 mode = insanity;
	}

	pApp->Generate(mode, algo, lops, siz, Colors); // Generate.

	ViewImage("raw.bmp");
}

// Save generated 'Pressed'.
void MainWindow::btnGenSaveOnPressed(void)
{
	QString Filename;

	// Get filename..
	Filename = QFileDialog::getSaveFileName(this, tr("Save generated maze"), nullptr, tr("Text(*.txt);;Scalable Vector Graphics(*.svg)"));
	if(Filename.isNull()) // Cancled..
	{
		return;
	}

	if(Filename.endsWith(".txt")) // Text.
	{
		pApp->SaveTxt(Filename.toLocal8Bit().data(), generated);
	}
	else if(Filename.endsWith(".svg")) // SVG.
	{
		pApp->SaveSvg(Filename.toLocal8Bit().data(), generated);
	}
	else // Unrecognized.
	{
		pConsole->Print("Error!: Unrecognized format!.\n");
	}
}

// Load generated 'Pressed'.
void MainWindow::btnGenLoadOnPressed(void)
{
	QString Filename;

	// Get filename..
	Filename = QFileDialog::getOpenFileName(this, tr("Load generated maze"), nullptr, tr("Text(*.txt);;Scalable Vector Graphics(*.svg)"));
	if(Filename.isNull()) // Cancled..
	{
		return;
	}

	if(Filename.endsWith(".txt")||Filename.endsWith(".TXT")) // Text.
	{
		pApp->LoadTxt(Filename.toLocal8Bit().data());
	}
	else if(Filename.endsWith(".svg")||Filename.endsWith(".SVG")) // SVG.
	{
		pApp->LoadSvg(Filename.toLocal8Bit().data());
	}
	else // Unrecognized.
	{
		pConsole->Print("Error!: Unrecognized format!.\n");
		return;
	}

	// Refresh.
	pApp->SetColor(Colors);
	if(pApp->IsSolved()) // Solved.
	{
		ViewImage("solved.bmp");
	}
	else // Raw.
	{
		ViewImage("raw.bmp");
	}
}

// Solve 'Pressed'.
void MainWindow::btnSolvOnPressed(void)
{
	int mode, fadeDir;

	mode = secret;

	fadeDir = none;
	pApp->Solve(mode, Colors, fadeDir);

	ViewImage("solved.bmp");
}

// Save solved 'Pressed'.
void MainWindow::btnSolvSaveOnPressed(void)
{
	QString Filename;

	// Get filename..
	Filename = QFileDialog::getSaveFileName(this, tr("Save solved maze"), nullptr, tr("Text(*.txt);;Scalable Vector Graphics(*.svg)"));
	if(Filename.isNull()) // Cancled..
	{
		return;
	}

	if(Filename.endsWith(".txt")) // Text.
	{
		pApp->SaveTxt(Filename.toLocal8Bit().data(), final_maze);
	}
	else if(Filename.endsWith(".svg")) // SVG.
	{
		pApp->SaveSvg(Filename.toLocal8Bit().data(), final_maze);
	}
	else // Unrecognized.
	{
		pConsole->Print("Error!: Unrecognized format!.\n");
	}
}

/*
 * UI window events.
 */

// Window size change.
void MainWindow::resizeEvent(QResizeEvent *_pQResizeEvent)
{
	QMainWindow::resizeEvent(_pQResizeEvent);
}

/*
 * Functions.
 */

// Reset colors.
void MainWindow::ClrRst(void)
{
	QColor color;

	// Wall, default color: black
	Colors[wall].Red = 0;
	Colors[wall].Green = 0;
	Colors[wall].Blue = 0;
	color.setRgb(Colors[wall].Red, Colors[wall].Green, Colors[wall].Blue);
	ui->btnClrWall->setStyleSheet( "background-color: " + color.name() );
	// Path, default color: white	Colors[path].Red = 255;
	Colors[path].Red = 255;
	Colors[path].Green = 255;
	Colors[path].Blue = 255;
	color.setRgb(Colors[path].Red, Colors[path].Green, Colors[path].Blue);
	ui->btnClrPath->setStyleSheet( "background-color: " + color.name() );
	// Recursive solution, default color: red
	Colors[rec_path].Red = 255;
	Colors[rec_path].Green = 0;
	Colors[rec_path].Blue = 0;
	color.setRgb(Colors[rec_path].Red, Colors[rec_path].Green, Colors[rec_path].Blue);
	ui->btnClrRecPath->setStyleSheet( "background-color: " + color.name() );
	// BFS solution, default color: blue
	Colors[bfs_path].Red = 0;
	Colors[bfs_path].Green = 0;
	Colors[bfs_path].Blue = 255;
	color.setRgb(Colors[bfs_path].Red, Colors[bfs_path].Green, Colors[bfs_path].Blue);
	ui->btnClrBfsPath->setStyleSheet( "background-color: " + color.name() );
	// Solution crossover, default color: purple
	Colors[crossover].Red = 128;
	Colors[crossover].Green = 0;
	Colors[crossover].Blue = 128;
	color.setRgb(Colors[crossover].Red, Colors[crossover].Green, Colors[crossover].Blue);
	ui->btnClrCros->setStyleSheet( "background-color: " + color.name() );
}

// Display image file.
int MainWindow::ViewImage(const char *_Filename)
{
	QPixmap pixmap(_Filename);
	ui->lblImg->setPixmap(pixmap);
	return 0;
}

void MainWindow::ChangeDisplayedColor(void)
{
	short setting = pApp->SetColor(Colors);
	if(setting == 2)
		ViewImage("solved.bmp");
	else if(setting == 1)
		ViewImage("raw.bmp");
}

//--------------------------
