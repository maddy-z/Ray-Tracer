#include <cmath>
#include <cfloat>

#include "Common.h"
#include "RayTracer.h"

// =====================================================
//  Static Global Variables used to parse the arguments 
// =====================================================

char *input_file  = NULL;
char *output_file = NULL;

int imageWidth  = 100;
int imageHeight = 100;

int num_bounces = 5;
float weight = 0.01f;

// ==============
//  Test Program
// ==============

int main(int argc, char ** argv)
{

	// Sample Command Lines:
	// raytracer -input scene.txt -size 1000 1000 -output image.tga -bounces 5 -weight 0.01

	for (int i = 1; i < argc; ++i)
	{
		if (!strcmp(argv[i], "-input")) 
		{
			i++; assert (i < argc); 
			input_file = argv[i];
		}
		else if (!strcmp(argv[i], "-size")) 
		{
			i++; assert (i < argc); 
			imageWidth = atoi(argv[i]);
			
			i++; assert (i < argc);
			imageHeight = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-output")) 
		{
			i++; assert (i < argc);
			output_file = argv[i];
		}
		else if (!strcmp(argv[i], "-bounces")) 
		{
			i++; assert (i < argc);
			num_bounces = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-weight"))
		{
			i++; assert (i < argc);
			weight = (float)(atof(argv[i]));
		}
		else 
		{
			printf ("Whoops error with command line argument %d: '%s'\n", i, argv[i]);
			assert(0);
		}
	}

	printf("Command Arguments Information:\n");
	printf("\t-input  %s\n", input_file);
	printf("\t-size  %d %d\n", imageHeight, imageWidth);
	printf("\t-output  %s\n", output_file);
	printf("\t-bounces  %d\n", num_bounces);
	printf("\t-weight  %f\n", weight);

	cout << endl;

	// ======================
	//  Scene Initialization
	// ======================

	SceneParser sparser(input_file);
	std::cout << sparser.toString() << std::endl;

	Image img( imageWidth, imageHeight );
	img.SetAllPixels(sparser.getBackgroundColor());
	
	RayTracer rayTracer( &sparser, num_bounces, weight );
	Camera * cam = sparser.getCamera();

	std::cout << "Start to Generate Ray" << endl << endl;

	for (int i = 0; i < imageHeight; ++i)
	{
		for (int j = 0; j < imageWidth; ++j)
		{
			Vec2f cameraPoint( (float)(i) / (float)(imageHeight), (float)(j) / (float)(imageWidth) );
			Ray ray = cam -> generateRay(cameraPoint);
			Hit hitPoint ( FLT_MAX, NULL, Vec3f(0.0f, 1.0f, 0.0f) );

			Vec3f pixel = rayTracer.traceRay(ray, EPSILON, 0, weight, 1.0f, hitPoint);
			if (pixel[0] == -1.0f && pixel[1] == -1.0f && pixel[2] == -1.0f)
				continue;

			if ( pixel[0] > 1.0f ) pixel.Set( 1.0f, pixel[1], pixel[2] );
			if ( pixel[1] > 1.0f ) pixel.Set( pixel[0], 1.0f, pixel[2] );
			if ( pixel[2] > 1.0f ) pixel.Set( pixel[0], pixel[1], 1.0f );
				
			img.SetPixel(j, i, pixel);
		}
	}

	if (output_file != NULL) {
		img.SaveTGA(output_file);
	}

	// system("pause");
	
	return 0;

}