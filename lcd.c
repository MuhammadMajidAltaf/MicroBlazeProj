void drawPixel(int x, int y){
	setXY(x, y, x, y);
	LCD_Write_DATA16(fch, fcl);
	clrXY();
}

void drawCircle(int x0, int y0, int r)
{

	int f = 1 - r;
	int ddF_x = 1;
	int ddF_y = -2 * r;
	int x = 0;
	int y = r;


	drawPixel(x0, y0 + r);
	drawPixel(x0, y0 - r);
	drawPixel(x0 + r, y0);
	drawPixel(x0 - r, y0);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		drawPixel(x0 + x, y0 + y);
		drawPixel(x0 - x, y0 + y);
		drawPixel(x0 + x, y0 - y);
		drawPixel(x0 - x, y0 - y);
		drawPixel(x0 + y, y0 + x);
		drawPixel(x0 - y, y0 + x);
		drawPixel(x0 + y, y0 - x);
		drawPixel(x0 - y, y0 - x);
	}

}

void drawFilledCircle(int x0, int y0, int r) {
	int f = 1 - r;
	int ddF_x = 1;
	int ddF_y = -2 * r;
	int x = 0;
	int y = r;

    drawPixel(x0, y0 + r);
    drawPixel(x0, y0 - r);
    drawPixel(x0 + r, y0);
    drawPixel(x0 - r, y0);
    drawHLine(x0 - r, y0, 2 * r);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        drawHLine(x0 - x, y0 + y, 2*x);
        drawHLine(x0 - x, y0 - y, 2*x);

        drawHLine(x0 - y, y0 + x, 2*y);
        drawHLine(x0 - y, y0 - x, 2*y);
    }
}
