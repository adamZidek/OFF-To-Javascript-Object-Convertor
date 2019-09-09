/*
	Adam Zidek
	CSCI 360
	Homework 4
	Off Putting
	HELPED BY Mike Lin
	Math explained to me by Gabe
*/
#include <string>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <sstream>

using namespace std;

string DEFAULT_MODEL_NAME = "CubeModel";

struct Point
{
    float x;
    float y;
    float z;
};

struct OffFormat
{
	int numVertices;
	int numFaces;
	int numEdges;
	vector<Point> vertices;
	vector<int> faces;
    vector <int> numVertsPerFace;
};

void OutputModel(OffFormat model, string modelName);
void DecomposeOffFile(OffFormat & model);
float GetMinX(OffFormat model);
float GetMaxX(OffFormat model);
float GetMinY(OffFormat model);
float GetMaxY(OffFormat model);
float GetMinZ(OffFormat model);
float GetMaxZ(OffFormat model);
void ScaleModel(OffFormat & model);
float GetMax(float max1, float max2, float max3);


int main(int argc, char * argv[])
{
	string modelName = DEFAULT_MODEL_NAME;			//Assign default model name(Will be changed if command line is entered)	
	OffFormat model;
	
	if(strncmp(argv[1], "-modelName", 100) == 0)
	{
		modelName = argv[2];
	}
	
	DecomposeOffFile(model);
    ScaleModel(model);
    OutputModel(model,modelName);
	
	return 0;
}

void OutputModel(OffFormat model, string modelName)
{
    /******************TRIANGEL OBJECT********************/
    int coord = 0;
	int temp = 0;
	
    cout<<"function "<<modelName<<"() {"<<endl;
    cout<<"this.Triangles = [";
    
    for(int i = 0; i < model.faces.size(); i++)
    {
        int coord = model.faces[temp];

        cout<<"["<<model.vertices[coord].x<<", ";
        cout<<model.vertices[coord].y<<", ";
        cout<<model.vertices[coord].z<<"],";
		temp++;
    }
    cout<<"];"<<endl<<endl;
    
    /*********************************BARYCENTRIC SHADER OBJECT*****************/

    cout<<"this.BC = [";
    
    for(int i = 0; i < model.faces.size(); i++)
    {
        cout<<"[1, 0, 0], [0, 1, 0], [0, 0, 1] ,";
    }
    
    cout<<"];"<<endl<<endl;
    cout<<"};"<<endl;
}

float GetMinX(OffFormat model)
{
    float min = model.vertices[0].x;        //Assign first value
    
    for(int i = 1; i < model.vertices.size(); i++)
    {
        if(model.vertices[i].x < min)
        {
            min = model.vertices[i].x;
        }
    }
    return min;
}

float GetMax(float max1, float max2, float max3)
{
    float max = max1;
	
	if(max2 > max)
	{
		max = max2;
	}
	else if(max3 > max)
	{
		max = max2;
	}
	
	return max;
}

float GetMaxX(OffFormat model)
{
    float max = model.vertices[0].x;    //Assign first value
    
    for(int i = 1; i < model.vertices.size(); i++)
    {
        if(model.vertices[i].x > max)
        {
            max = model.vertices[i].x;
        }
    }
    return max;
}

float GetMinY(OffFormat model)
{
    float min = model.vertices[0].y;    //Assign first value
    
    for(int i = 1; i < model.vertices.size(); i++)
    {
        if(model.vertices[i].y < min)
        {
            min = model.vertices[i].y;
        }
    }
    return min;
}

float GetMaxY(OffFormat model)
{
    float max = model.vertices[0].y;    //Assign first value
    
    for(int i = 1; i < model.vertices.size(); i++)
    {
        if(model.vertices[i].y > max)
        {
            max = model.vertices[i].y;
        }
    }
    return max;
}

float GetMinZ(OffFormat model)
{
    float min = model.vertices[0].z;    //Assign first value
    
    for(int i = 1; i < model.vertices.size(); i++)
    {
        if(model.vertices[i].z < min)
        {
            min = model.vertices[i].z;
        }
    }
    return min;
}

float GetMaxZ(OffFormat model)
{
    float max = model.vertices[0].z;    //Assign first value
    
    for(int i = 1; i < model.vertices.size(); i++)
    {
        if(model.vertices[i].z > max)
        {
            max = model.vertices[i].z;
        }
    }
    return max;
}

void ScaleModel(OffFormat & model)
{
    //Get Max and Mins
    float minX = GetMinX(model);
    float minY = GetMinY(model);
    float minZ = GetMinZ(model);
    
    //Invert mins
    float minXInvert = -1 * minX;
    float minYInvert = -1 * minY;
    float minZInvert = -1 * minZ;
    
    //Add inverted min to all coordiates
    for(int i = 0; i < model.numVertices; i++)
    {
        model.vertices[i].x += minXInvert;
        model.vertices[i].y += minYInvert;
        model.vertices[i].z += minZInvert;
    }
    
    float maxX = GetMaxX(model);
    float maxY = GetMaxY(model);
    float maxZ = GetMaxZ(model);
	float MAX = GetMax(maxX, maxY, maxZ);
    
    //Scale all coordiates by the max
    for(int i = 0; i < model.numVertices; i++)
    {
        model.vertices[i].x /= MAX;
        model.vertices[i].y /= MAX;
        model.vertices[i].z /= MAX;
    }
    
    //Subtract all coordinates by .5
    for(int i = 0; i < model.numVertices; i++)
    {
        model.vertices[i].x -= .5;
        model.vertices[i].y -= .5;
        model.vertices[i].z -= .5;
    }
    return;
}

void DecomposeOffFile(OffFormat & model)
{
	string marker;
    string verts;
    string faces;
    int face;
    string junk;
    int numFacesInVert;
    //int maxCount = 0;
	
	getline(cin, marker, '\n');		//Grabs the OFF from file since we dont need it
	
    //IF WE HAVENT REACHED A NUMBER THERE ARE COMMENTS
    while(!(cin >> model.numVertices))
    {
        cin.clear();
        getline(cin, marker, '\n');
    }
    
    //Grab Faces and edges, throw away new line
	cin >> model.numFaces;
	cin >> model.numEdges;
    getline(cin, marker, '\n');
    
    
	//Get all vertices
	for(int i = 0; i < model.numVertices; i++)
	{
		getline(cin, verts, '\n');
        stringstream s(verts);      //Holds line of vertices
        Point temp;
        
        s >> temp.x;
        s >> temp.y;
        s >> temp.z;
        
        model.vertices.push_back(temp);
        s.str(string());
	}
	
	
	
	//Grab all faces, also perform the fan alogorith
	for(int i = 0; i < model.numFaces; i++)
	{
        //Check if there are any comments between vertices and faces
        while(!(cin >> numFacesInVert))
        {
            cin.clear();
            getline(cin, marker, '\n');
        }
        //cin >> numFacesInVert;
        model.numVertsPerFace.push_back(numFacesInVert);
        getline(cin, faces, '\n');
        stringstream s(faces);      //Holds line of faces
        vector<int> tempFaces;
        
        //Push faces into temp vector
        while(s >> face)
        {
            tempFaces.push_back(face);
        }
        
        //If its a triangle we dont want to perform a fan on it
        if(numFacesInVert == 3)
        {
            model.faces.push_back(tempFaces[0]);
            model.faces.push_back(tempFaces[1]);
            model.faces.push_back(tempFaces[2]);
        }
        //If its greater than 3, fan, and push new triangles to structure
        else
        {
            for(int j = 0; j < numFacesInVert -2; j++)
            {
                model.faces.push_back(tempFaces[0]);
                model.faces.push_back(tempFaces[j+2]);
                model.faces.push_back(tempFaces[j+1]);
            }
        }
        //Clear the temp and string stream
        tempFaces.clear();
        s.str(string());
	}
}
