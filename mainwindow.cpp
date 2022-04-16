#include "mainwindow.h"
#include "ui_mainwindow.h"


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
	connect(ui->radAlgo1, &QPushButton::pressed, this, &MainWindow::btnAlg1OnPressed); // Algo1 'Pressed'.
	connect(ui->radAlgo2, &QPushButton::pressed, this, &MainWindow::btnAlg2OnPressed); // Algo2 'Pressed'.
	connect(ui->radAlgo3, &QPushButton::pressed, this, &MainWindow::btnAlg3OnPressed); // Algo3 'Pressed'.
	connect(ui->spnSiz, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::spnSizOnChanged); // Maze size 'Changed'.
	connect(ui->btnGen, &QPushButton::pressed, this, &MainWindow::btnGenOnPressed); // Generate 'Pressed'.
	connect(ui->btnClrWall, &QPushButton::pressed, this, &MainWindow::btnClrWallOnPressed); // Color walls 'Pressed'.
	connect(ui->btnClrPath, &QPushButton::pressed, this, &MainWindow::btnClrPathOnPressed); // Color path 'Pressed'.
	connect(ui->btnClrRecPath, &QPushButton::pressed, this, &MainWindow::btnClrRecPathOnPressed); // Color recursive path 'Pressed'.
	connect(ui->btnClrBfsPath, &QPushButton::pressed, this, &MainWindow::btnClrBfsPathOnPressed); // Color bfs path 'Pressed'.
	connect(ui->btnClrCros, &QPushButton::pressed, this, &MainWindow::btnClrCrosOnPressed); // Color crosover 'Pressed'.
	connect(ui->btnClrFadeS, &QPushButton::pressed, this, &MainWindow::btnClrFadeSOnPressed); // Color fade start 'Pressed'.
	connect(ui->btnClrFadeE, &QPushButton::pressed, this, &MainWindow::btnClrFadeEOnPressed); // Color fade end 'Pressed'.
	connect(ui->btnClrRst, &QPushButton::pressed, this, &MainWindow::btnClrRstOnPressed); // Color reset 'Pressed'.
	connect(ui->btnSolv, &QPushButton::pressed, this, &MainWindow::btnSolvOnPressed); // Solve 'Pressed'.

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

//p

/*
 * UI signal procedures.
 */

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
    // This may interfere with direct manual input!.. uhhhhhhhh
	if((_Val % 2) == 0) // Even!
	{
		_Val++; // Odd it.
		ui->spnSiz->setValue(_Val); // Set it.
	}
}

// Generate 'Pressed'.
void MainWindow::btnGenOnPressed(void)
{
	int algo, lops, siz;

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

	pApp->Generate(algo, lops, siz, Colors); // Generate.

	ViewImage("raw.bmp");
}

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
}

// Color fade start 'Pressed'.
void MainWindow::btnClrFadeSOnPressed(void)
{
	QColor color, colorNew;

	color.setRgb(Colors[fade_start].Red, Colors[fade_start].Green, Colors[fade_start].Blue);

	colorNew = QColorDialog::getColor(color, parentWidget());
	if(!colorNew.isValid())
		return;

	Colors[fade_start].Red = colorNew.red();
	Colors[fade_start].Green = colorNew.green();
	Colors[fade_start].Blue = colorNew.blue();

	color.setRgb(Colors[fade_start].Red, Colors[fade_start].Green, Colors[fade_start].Blue);
	ui->btnClrFadeS->setStyleSheet("background-color: " + color.name());
}

// Color fade end 'Pressed'.
void MainWindow::btnClrFadeEOnPressed(void)
{
	QColor color, colorNew;

	color.setRgb(Colors[fade_end].Red, Colors[fade_end].Green, Colors[fade_end].Blue);

	colorNew = QColorDialog::getColor(color, parentWidget());
	if(!colorNew.isValid())
		return;

	Colors[fade_end].Red = colorNew.red();
	Colors[fade_end].Green = colorNew.green();
	Colors[fade_end].Blue = colorNew.blue();

	color.setRgb(Colors[fade_end].Red, Colors[fade_end].Green, Colors[fade_end].Blue);
	ui->btnClrFadeE->setStyleSheet("background-color: " + color.name());
}

// Color reset 'Pressed'
void MainWindow::btnClrRstOnPressed(void)
{
	ClrRst();
	ui->radFadeN->setChecked(true);
	ui->chkClrIns->setChecked(false);
}

// Solve 'Pressed'.
void MainWindow::btnSolvOnPressed(void)
{
	int mode, fadeDir;

	mode = secret;
	if(ui->chkClrIns->isChecked())
	{
		mode = insanity;
	}

	fadeDir = none;
	/*
	if(ui->radFadeH->isChecked())
	{
		fadeDir = horizontal;
	}
	if(ui->radFadeV->isChecked())
	{
		fadeDir = vertical;
	}
	*/
	pApp->Solve(mode, Colors, fadeDir);

	ViewImage("solved.bmp");
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
	// Fade start
	Colors[fade_start].Red = 0;
	Colors[fade_start].Green = 64;
	Colors[fade_start].Blue = 64;
	color.setRgb(Colors[fade_start].Red, Colors[fade_start].Green, Colors[fade_start].Blue);
	ui->btnClrFadeS->setStyleSheet( "background-color: " + color.name() );
	// Fade end
	Colors[fade_end].Red = 0;
	Colors[fade_end].Green = 220;
	Colors[fade_end].Blue = 220;
	color.setRgb(Colors[fade_end].Red, Colors[fade_end].Green, Colors[fade_end].Blue);
	ui->btnClrFadeE->setStyleSheet( "background-color: " + color.name() );
}

// Display image file.
int MainWindow::ViewImage(const char *_Filename)
{
	QPixmap pixmap(_Filename);
	ui->lblImg->setPixmap(pixmap);
	return 0;
}

//----------
