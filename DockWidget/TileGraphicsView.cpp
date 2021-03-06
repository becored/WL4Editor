﻿#include "TileGraphicsView.h"
#include "LevelComponents/Tileset.h"

#include <QMouseEvent>
#include <QScrollBar>
#include <QTextStream>

static bool initialized = false; // Used to allow the vertical scrollbar to initialize to the top once.

/// <summary>
/// Select the tile that was clicked.
/// </summary>
/// <remarks>
/// Update the tile info text.
/// Set the tile ID that is now being highlighted by the parent dock widget.
/// If the user clicks out of bounds of the selectable tiles, do nothing.
/// </remarks>
/// <param name="event">
/// The object from which the mouse press position can be obtained.
/// </param>
void TileGraphicsView::mousePressEvent(QMouseEvent *event)
{
    holdingmouse = true;

    // Get the ID of the tile that was clicked
    int X = event->x() + horizontalScrollBar()->sliderPosition();
    int Y = event->y() + verticalScrollBar()->sliderPosition();
    if (X >= 32 * 8 || Y >= 32 * 0x60)
        return;
    int tileX = X >> 5;
    int tileY = Y >> 5;
    if(tx == tileX && ty == tileY)
        return;
    int tileID = tileX + tileY * 8;
    tx = tileX;
    ty = tileY;

    // Set the selected tile location for the graphics view
    Map16DockWidget->SetSelectedTile((unsigned short) tileID, false);
}

/// <summary>
/// this function will be called when the mouse is moving on the graphic view in the dock widget.
/// </summary>
/// <param name="event">
/// The mouse click event.
/// </param>
void TileGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if(!holdingmouse) return;

    int X = event->x() + horizontalScrollBar()->sliderPosition();
    int Y = event->y() + verticalScrollBar()->sliderPosition();
    if (X >= 32 * 8 || Y >= 32 * 0x60)
        return;
    int tileX = X >> 5;
    int tileY = Y >> 5;
    if(tx > tileX || ty > tileY) // you cannot drag to directions if it not right-down side
        return;

    Map16DockWidget->RectSelectTiles(tileX - tx + 1, tileY - ty + 1);
}

/// <summary>
/// this function will be called when the mouse is dragging out from the graphic view in the dock widget.
/// </summary>
/// <param name="event">
/// The drag leave event.
/// </param>
void TileGraphicsView::dragLeaveEvent(QDragLeaveEvent *event)
{
    (void) event;
    holdingmouse = false;
}

/// <summary>
/// this function will be called when the graphic view in the dock widget is clicked then mouse release.
/// </summary>
/// <param name="event">
/// The mouse click event.
/// </param>
void TileGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    (void) event;
    holdingmouse = false;
}

/// <summary>
/// Show the graphics view, and set the vertical scrollbar to the top of the view.
/// </summary>
/// <remarks>
/// Without this override, the graphics view will initialize with the vertical scrollbar halfway down.
/// </remarks>
/// <param name="event">
/// Resize event information which is sent to the parent implementation of this function.
/// </param>
void TileGraphicsView::showEvent(QShowEvent *event)
{
    QGraphicsView::showEvent(event);
    if (!initialized)
    {
        initialized = true;
        verticalScrollBar()->setValue(0);
    }
}
