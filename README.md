# GridStarLayout
GridStarLayout is a Qt layout with a design similar to that of WPF's Grid with "star sizing", where each row and column is defined as a weighted proportion of the alloted space.

Example usage:
```
GridDefinition
    mainGridDef,
    subGridDef;

mainGridDef.addRowDefinition(0.03f);
mainGridDef.addRowDefinition(0.08f);
mainGridDef.addRowDefinition(0.94f);
mainGridDef.addRowDefinition(0.08f);
mainGridDef.addRowDefinition(0.03f);

mainGridDef.addColumnDefinition(0.03f);
mainGridDef.addColumnDefinition(0.2f);
mainGridDef.addColumnDefinition(0.03f);
mainGridDef.addColumnDefinition();

subGridDef.addRowDefinition(0.03f);
subGridDef.addRowDefinition(0.15f);
subGridDef.addRowDefinition();
subGridDef.addRowDefinition(0.03f);

subGridDef.addColumnDefinition(0.2f);
subGridDef.addColumnDefinition(0.5f);
subGridDef.addColumnDefinition(0.2f);

GridStarLayout
    *mainGrid = new GridStarLayout(mainGridDef);

QLabel *mainBg = new QLabel();
mainBg->setStyleSheet("* { background: rgba(200, 100, 150, 255); }");

mainGrid->addWidget(mainBg, 0, 0, 0, 0);
mainGrid->addWidget(new QPushButton("Test"), 1, 1);
mainGrid->addWidget(new QPushButton("Test"), 3, 1);

GridStarLayout
    *subGrid = new GridStarLayout(subGridDef);

QLabel *subBg = new QLabel();
subBg->setStyleSheet("* { background: rgba(100, 150, 200, 255); }");

subGrid->addWidget(subBg, 0, 0, 0, 0);
subGrid->addWidget(new QScrollBar(Qt::Orientation::Horizontal), 1, 1);

mainGrid->addLayout(subGrid, 0, 3, 0);

centralWidget()->setLayout(mainGrid);
```
