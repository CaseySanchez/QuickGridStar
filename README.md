# QML GridStar Layout

---

### Description
The GridStar layout is a clone of the Windows Presentation Foundation's "Grid" type with star sizing capabilities, where each row and column is defined as a weighted proportion of the available space. This allows absolute control over how you want your layout to look.

---

### RowDefinition Type
#### Properties
| Name | Type | Description |
| --- |---| ---|
| `weight` | `real` | Defines a weighted proportion of the GridStar's rows. Default value is `1.0`. |

### ColumnDefinition Type
#### Properties
| Name | Type | Description |
| --- |---| ---|
| `weight` | `real` | Defines a weighted proportion of the GridStar's columns. Default value is `1.0`. |

### GridStar Type
#### Attached Properties
| Name | Type | Description |
| --- |---| ---|
| `ignore` | `bool` | Set to `true` if you want this attached Item to not be positioned by the layout. Default value is `false`.
| `row` | `int` | Specifies the row in which the attached Item is positioned. Default value is `0`. |
| `column` | `int` | Specifies the column in which the attached Item is positioned. Default value is `0`.  |
| `rowSpan` | `int` | Defines how many rows the attached Item will span/occupy. Default value is `1`. Values less than or equal to `0` will span the total remaining space minus that value. |
| `columnSpan` | `int` | Defines how many columns the attached Item will span/occupy. Default value is `1`. Values less than or equal to `0` will span the total remaining space minus that value.  |
#### Methods
| Name | Return Type | Parameters | Description |
| --- | --- | --- | --- |
| `rowCount` | `int` | `None` | Returns the number of rows in the grid definition. |
| `columnCount` | `int` | `None` | Returns the number of columns in the grid definition. |
| `itemsAt` | `QVariant` | `int row, int column` | Returns a Javascript array of the items at the cell of `row` and `column`. |
| `addItem` | `void` | `QObject *object, int row = 0, int column = 0, int rowSpan = 1, int columnSpan = 1` | Adds `object` to the grid at the cell of `row` and `column` with the spans of `rowSpan` and `columnSpan`. |
| `removeItem` | `bool` | `QObject *object` | Removes `object` from the grid. |
| `addRowDefinition` | `void` | `float weight = 1.0f, int row = -1` | Adds a row definition of `weight` at `row`, default `row` value of `-1` adds the definition to the end. |
| `addColumnDefinition` | `void` | `float weight = 1.0f, int column = -1` | Adds a column definition of `weight` at `column`, default `column` value of `-1` adds the definition to the end. |
| `removeRowDefinition` | `void` | `int row = -1` | Removes the definition at `row`, default `row` value of `-1`removes the last definition. |
| `removeColumnDefinition` | `void` | `int column = -1` | Removes the definition at `column`, default `column` value of `-1`removes the last definition. |

---

### Registration
```
#include <QApplication>
#include <QQmlApplicationEngine>

#include "quickgridstar.h"

int main(int argc, char *argv[])
{
    QApplication
        app(argc, argv);

    QQmlApplicationEngine
        engine;

    QuickGridStar::registerTypes();

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
```

---

### QML Example
```
import QtQuick 2.3
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.4

// IMPORTANT!
import com.quick.gridStar 1.0

ApplicationWindow
{
    visible: true
    width: 1280
    height: 720
    title: qsTr("GridStar")

    GridStar
    {
        RowDefinition { weight: 0.03 }
        RowDefinition { weight: 0.8 }
        RowDefinition { weight: 0.03 }

        ColumnDefinition { weight: 0.03 }
        ColumnDefinition { weight: 0.2 }
        ColumnDefinition { weight: 0.03 }
        ColumnDefinition { weight: 0.03 }
        ColumnDefinition { }
        ColumnDefinition { weight: 0.03 }

        Rectangle
        {
            GridStar.row: 0
            GridStar.column: 0
            GridStar.rowSpan: 0
            GridStar.columnSpan: 0

            color: "#CFCFCF"
        }

        Rectangle
        {
            GridStar.row: 0
            GridStar.column: 3
            GridStar.rowSpan: 0 // Span remaining rows
            GridStar.columnSpan: 3

            color: "#DDDDDD"
        }

        GridStar
        {
            GridStar.row: 1
            GridStar.column: 4
            GridStar.rowSpan: -1 // Span remaining rows minus 1

            RowDefinition { weight: 0.2 }
            RowDefinition { weight: 0.8 }

            ColumnDefinition { weight: 0.2 }
            ColumnDefinition { weight: 0.4 }
            ColumnDefinition { weight: 0.2 }

            Button
            {
                GridStar.row: 0
                GridStar.column: 1

                text: "Sub Grid Button"
            }
        }

        GridStar
        {
            GridStar.row: 1
            GridStar.column: 1

            RowDefinition { weight: 0.1 }
            RowDefinition { weight: 0.05 }
            RowDefinition { weight: 0.1 }
            RowDefinition { weight: 0.05 }
            RowDefinition { weight: 0.1 }
            RowDefinition { weight: 0.5 }
            RowDefinition { weight: 0.1 }

            ColumnDefinition { }

            Button
            {
                GridStar.row: 0
                GridStar.column: 0

                text: "Grid Button 1"
            }

            Button
            {
                GridStar.row: 2
                GridStar.column: 0

                text: "Grid Button 2"
            }

            Button
            {
                GridStar.row: 4
                GridStar.column: 0

                text: "Grid Button 3"
            }

            Button
            {
                GridStar.row: 6
                GridStar.column: 0

                text: "Grid Button 4"
            }
        }
    }
}
```

---

### Result
![](http://i.imgur.com/oAf18gO.png)

---

Happy coding Qt community!
