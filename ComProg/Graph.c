#include "pbPlots.h"
#include "supportLib.h"


void Plot(double xs[],double ys[])
{
    _Bool success;

	StartArenaAllocator();

	RGBABitmapImageReference *canvasReference = CreateRGBABitmapImageReference();
	StringReference *errorMessage = CreateStringReference(L"", 0);
	success = DrawScatterPlot(canvasReference, 600, 400, xs, 5, ys, 5, errorMessage);

	if(success){
		size_t length;
		ByteArray *pngdata = ConvertToPNG(canvasReference->image);
		WriteToFile(pngdata, "Graph.png");
		DeleteImage(canvasReference->image);
	}else{
		fprintf(stderr, "Error: ");
		for(int i = 0; i < errorMessage->stringLength; i++){
			fprintf(stderr, "%c", errorMessage->string[i]);
		}
		fprintf(stderr, "\n");
	}

	FreeAllocations();

}

int main(){
	double xs [] = {69, 2, 3, 4, 67};
	double ys [] = {2, 4, 6, 8, 69420};

	Plot(xs,ys);

}
//png อยู่ใน ComProg\comprog\Graph.png
