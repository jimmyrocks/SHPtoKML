#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip> //Used to set the precision of the double output to the KML file
using namespace std;
typedef signed long int __int32;


void TrimSpaces( string& str)
{  
	// Trim Both leading and trailing spaces  
	size_t startpos = str.find_first_not_of(" \t"); // Find the first character position after excluding leading blank spaces  
	size_t endpos = str.find_last_not_of(" \t"); // Find the first character position from reverse af  
	
	// if all spaces or empty return an empty string  
	if(( string::npos == startpos ) || ( string::npos == endpos))  
	{  
		str = "";  
	}  
	else  
		str = str.substr( startpos, endpos-startpos+1 );  
	
	//Remove any non-ascii viewable characters
	std::stringstream sStringBufferTrim;
	int temp;
	for (int i=0; i< str.length(); i++)
	{
		temp = (str)[i];
		if ((temp > 31) && (temp < 127))
		{
			sStringBufferTrim << str[i];
		}
	}
	
	string sNewTrim(sStringBufferTrim.str());
	str =sNewTrim;
}

int liOpenFile(string lsFileName, string lsFilePath)
{
	string lsFileShape(lsFilePath + lsFileName + ".shp");
	string lsFileDBF(lsFilePath + lsFileName + ".dbf");
	
	//Declare the SHP file and open it
	char * lcFileName = new char[lsFileShape.size()];
	unsigned char * lcFileContents;
	char * lCFileContents;
	strcpy(lcFileName,lsFileShape.c_str());
	int liReturnValue = 0;	
	ifstream loFile (lcFileName, ios::in|ios::binary|ios::ate);
	
	if (loFile.is_open())
	{
		ifstream::pos_type size = loFile.tellg();
		lCFileContents = new char [size];
		loFile.seekg (0, ios::beg);
		loFile.read (lCFileContents, size);
		loFile.close();
		lcFileContents = (unsigned char*)lCFileContents;
		
		__int32 sShapeType =     (
								  lcFileContents[32]
								  + lcFileContents[23] * 256
								  + lcFileContents[34] * 256 * 256
								  + lcFileContents[35] * 256 * 256 * 256
								  );
		
		
		delete[] lcFileContents;
		
		if (sShapeType == 5)
		{
			
			liReturnValue++;
			
			//Declare the DBF file and open it
			ifstream::pos_type sizeDBF;
			char * lCFileContentsDBF;
			unsigned char * lcFileContentsDBF;
			char * lcFileNameDBF = new char[lsFileDBF.size()];
			strcpy(lcFileNameDBF,lsFileDBF.c_str());
			liReturnValue = 0;
			
			ifstream loFileDBF (lcFileNameDBF, ios::in|ios::binary|ios::ate);
			
			if (loFileDBF.is_open())
			{
				//Try to read the DBF
				bool lbDBFFound = false;
				if (loFileDBF.is_open())
				{
					sizeDBF = loFileDBF.tellg();
					lCFileContentsDBF = new char [sizeDBF];
					loFileDBF.seekg (0, ios::beg);
					loFileDBF.read (lCFileContentsDBF, sizeDBF);
					lbDBFFound = true;
					loFileDBF.close();
					lcFileContentsDBF = (unsigned char*)lCFileContentsDBF;
				}
				loFileDBF.close();
				
				//Look for the header info
				if (lbDBFFound)
				{
					
					__int32 DBFHeaderLength = 0;
					DBFHeaderLength =	lcFileContentsDBF[8] + 
					lcFileContentsDBF[9]*256;
					
					for (int i=0; i<(DBFHeaderLength-33); i=i+32)
					{
						//i = 32 + count, 0-MAX
						std::stringstream sStringBuffer1;
						sStringBuffer1 <<	 lcFileContentsDBF[32+i] <<  
						lcFileContentsDBF[33+i] <<  
						lcFileContentsDBF[34+i] <<  
						lcFileContentsDBF[35+i] <<  
						lcFileContentsDBF[36+i] <<  
						lcFileContentsDBF[37+i] <<  
						lcFileContentsDBF[38+i] <<  
						lcFileContentsDBF[39+i] <<  
						lcFileContentsDBF[40+i] <<  
						lcFileContentsDBF[41+i] <<  
						lcFileContentsDBF[42+i];
						
						//Display the Header info to the user
						string lsCurrentName(sStringBuffer1.str());
						TrimSpaces(lsCurrentName);
						cout << (i/32)+1 << ") " << lsCurrentName << endl;
					}
					
					delete[] lcFileContentsDBF;
				}
				liReturnValue++;
			}
			else
			{
				cout << "The selected DBF file: \"" << lsFileDBF << "\" Cannot be opened. " << "\n";
			}
		}
		else
		{
			//Since we only work with Polygon files, all other data will cause an error message
			
			//From Wikipedia
			// http://en.wikipedia.org/wiki/Shapefile
			
			cout << "The selected shapefile: \"" << lsFileShape << "\" is not supported at this time. " << "\n";
			cout << "Currently this program only supports Polygon data files (Type 5)" << "\n";
			
			
			switch ( sShapeType ) {
				case 0 : 
					cout << "The selected shapefile contains Null Shape data (Type " << sShapeType << ")" << endl;
					break;
				case 1 : 
					cout << "This shapefile contains Point data (Type " << sShapeType << ")" << endl;
					break;
				case 3 : 
					cout << "This shapefile contains Polyline data (Type " << sShapeType << ")" << endl;
					break;
				case 5 :  //Just included for the sake of keeping the data together
					cout << "This shapefile contains Polygon data (Type " << sShapeType << ")" << endl;
					break;
				case 8 : 
					cout << "This shapefile contains MultiPoint data (Type " << sShapeType << ")" << endl;
					break;
				case 11 : 
					cout << "This shapefile contains PointZ data (Type " << sShapeType << ")" << endl;
					break;
				case 13 : 
					cout << "This shapefile contains PolylineZ data (Type " << sShapeType << ")" << endl;
					break;
				case 15 : 
					cout << "This shapefile contains PolygonZ data (Type " << sShapeType << ")" << endl;
					break;	
				case 18 : 
					cout << "This shapefile contains MultiPointZ data (Type " << sShapeType << ")" << endl;
					break;	
				case 21 : 
					cout << "This shapefile contains PointM data (Type " << sShapeType << ")" << endl;
					break;	
				case 23 : 
					cout << "This shapefile contains PolylineM data (Type " << sShapeType << ")" << endl;
					break;	
				case 25 : 
					cout << "This shapefile contains PolygonM data (Type " << sShapeType << ")" << endl;
					break;
				case 28 : 
					cout << "This shapefile contains MultiPointM data (Type " << sShapeType << ")" << endl;
					break;
				case 31 : 
					cout << "This shapefile contains MultiPatch data (Type " << sShapeType << ")" << endl;
					break;
					
					liReturnValue = 0;
			}
		}
	}
	else
	{
		cout << "The selected shapefile: \"" << lsFileShape << "\" Cannot be opened. " << "\n";
	}
	
	
	if (liReturnValue>=1)
	{
		liReturnValue=1;
	}
	else
	{
		liReturnValue=0;
	}
	return liReturnValue;
}

int liLoadToMemory (string lsFileName, string lsFilePath, int iNameCol)
{
	string lsFileShape(lsFilePath + lsFileName + ".shp");
	string lsFileDBF(lsFilePath + lsFileName + ".dbf");
	
	//Declare the SHP file and open it
	ifstream::pos_type size;
	char * lCFileContents;
	unsigned char * lcFileContents;
	char * lcFileShape = new char[lsFileShape.size()];
	strcpy(lcFileShape,lsFileShape.c_str());
	
	//Declare the DBF file and open it
	ifstream::pos_type sizeDBF;
	char * lCFileContentsDBF;
	unsigned char * lcFileContentsDBF;
	char * lcFileNameDBF = new char[lsFileDBF.size()];
	strcpy(lcFileNameDBF,lsFileDBF.c_str());
	
	int liReturnValue = 0;
	
	///////////////
	// Shapefile File Format
	//http://en.wikipedia.org/wiki/Shapefile
	//http://www.esri.com/library/whitepapers/pdfs/shapefile.pdf
	//
	
	ifstream file (lcFileShape, ios::in|ios::binary|ios::ate);
	if (file.is_open())
	{
		size = file.tellg();
		lCFileContents = new char[size];
		file.seekg (0, ios::beg);
		file.read (lCFileContents, size);
		file.close();
		lcFileContents = (unsigned char*)lCFileContents;
		
		//Try to read the DBF
		bool lbDBFFound = false;
		ifstream fileDBF (lcFileNameDBF, ios::in|ios::binary|ios::ate);
		if (fileDBF.is_open())
		{
			sizeDBF = fileDBF.tellg();
			lCFileContentsDBF = new char [sizeDBF];
			fileDBF.seekg (0, ios::beg);
			fileDBF.read (lCFileContentsDBF, sizeDBF);
			lbDBFFound = true;
			fileDBF.close();
			lcFileContentsDBF = (unsigned char*)lCFileContentsDBF;
		}
		__int32 DBFHeaderLength = 0;
		__int32 DBFRecordLength = 0;
		__int32 DBFRecordCount = 0;
		if (lbDBFFound)
		{
			//Let's get the DBF headers and stuff
			//http://www.dbase.com/knowledgebase/int/db7_file_fmt.htm
			/*__int32 DBFRecordType = lcFileContentsDBF[0];
			 __int32 DBFRecordDateYY = lcFileContentsDBF[1]+1900;
			 __int32 DBFRecordDateMM = lcFileContentsDBF[2];
			 __int32 DBFRecordDateDD = lcFileContentsDBF[3]; */
			DBFRecordCount = 
			lcFileContentsDBF[4] +
			lcFileContentsDBF[5]*256 +
			lcFileContentsDBF[6]*256*256 +
			lcFileContentsDBF[7]*256*256*256;
			DBFHeaderLength =	lcFileContentsDBF[8] + 
			lcFileContentsDBF[9]*256;
			DBFRecordLength =	lcFileContentsDBF[10] +
			lcFileContentsDBF[11]*256;
			
			for (int i=0; i<(DBFHeaderLength-33); i=i+32)
			{
				//i = 32 + count, 0-MAX
				std::stringstream sStringBuffer1;
				sStringBuffer1 <<	 lcFileContentsDBF[32+i] <<  
				lcFileContentsDBF[33+i] <<  
				lcFileContentsDBF[34+i] <<  
				lcFileContentsDBF[35+i] <<  
				lcFileContentsDBF[36+i] <<  
				lcFileContentsDBF[37+i] <<  
				lcFileContentsDBF[38+i] <<  
				lcFileContentsDBF[39+i] <<  
				lcFileContentsDBF[40+i] <<  
				lcFileContentsDBF[41+i] <<  
				lcFileContentsDBF[42+i];
				
				string x(sStringBuffer1.str());
				std::stringstream sStringBuffer2;
				sStringBuffer2 << lcFileContentsDBF[43+i];
				x = sStringBuffer2.str();
			}
			//the records start at point ((DBFHeaderLength+64+1))
			
			//DBF File Debug Information
			//cout << "DBFRecordType: " << DBFRecordType << endl;
			//cout << "DBFRecordDateYY: " << DBFRecordDateYY << endl;
			//cout << "DBFRecordDateMM: " << DBFRecordDateMM << endl;
			//cout << "DBFRecordDateDD: " << DBFRecordDateDD << endl;
			//cout << "DBFRecordCount: " << DBFRecordCount << endl;
			//cout << "DBFHeaderLength: " << DBFHeaderLength << endl;
			//cout << "DBFRecordLength: " << DBFRecordLength << endl;
			
		}
		
		double dBounds[8];
		
		//0–3 	int32 	big 	File code (always hex value 0x0000270a)
		//__int32 sFileCode = lcFileContents[3] | (lcFileContents[2] << 8)  | (lcFileContents[1] << 16) | (lcFileContents[0] << 24);
		/*__int32 sFileCode = (
		 lcFileContents[0] * 256 * 256 * 256
		 + lcFileContents[1] * 256 * 256
		 + lcFileContents[2] * 256
		 + lcFileContents[3]
		 );*/
		//4–23 	int32 	big 	Unused; five uint32
		//24–27 	int32 	big 	File length (in 16-bit words, including the header)
		//__int32 sFileLength = lcFileContents[27] | (lcFileContents[26] << 8)  | (lcFileContents[25] << 16) | (lcFileContents[24] << 24);
		__int32 sFileLength = (
							   lcFileContents[24] * 256 * 256 * 256
							   + lcFileContents[25] * 256 * 256
							   + lcFileContents[26] * 256 
							   + lcFileContents[27]
							   );
		sFileLength = (sFileLength * 2 - 100)/8;
		//28–31 	int32 	little 	Version
		//__int32 sVersion = lcFileContents[28] | (lcFileContents[29] << 8)  | (lcFileContents[30] << 16) | (lcFileContents[31] << 24);
		/*__int32 sVersion =     (
		 lcFileContents[28]
		 + lcFileContents[29] * 256
		 + lcFileContents[30] * 256 * 256
		 + lcFileContents[31] * 256 * 256 * 256
		 ); */
		//32–35 	int32 	little 	Shape type (see reference below)
		//__int32 sShapeType = lcFileContents[32] | (lcFileContents[33] << 8)  | (lcFileContents[34] << 16) | (lcFileContents[35] << 24);
		__int32 sShapeType =     (
								  lcFileContents[32]
								  + lcFileContents[23] * 256
								  + lcFileContents[34] * 256 * 256
								  + lcFileContents[35] * 256 * 256 * 256
								  );
		//36–67 	double 	little 	Minimum bounding rectangle (MBR) of all shapes contained within the shapefile; four doubles in the following order: min X, min Y, max X, max Y
		
		for (int iCount=0; iCount<8; iCount++)
		{
			double dCopy;
			int iOffset = 36+(iCount*8);
			memcpy(&dCopy, lcFileContents + iOffset, 8);
			dBounds[(iCount)] = dCopy;
		}
		
		//Shapefile Debug information:
		//cout << "File Code:" << sFileCode << endl;
		//cout << "File Length:" << sFileLength << endl;
		//cout << "Version:" << sVersion << endl;
		//cout << "Shape Type:" << sShapeType << endl;
		//cout << "Mix WGS84 Bounds:" << dBounds[1] << ", " << dBounds[0] << endl;
		//cout << "Max WGS84 Bounds:" << dBounds[3] << ", " << dBounds[2] << endl;
		//cout << "Z Bounds (min, max):" << dBounds[4] << ", " << dBounds[5] << endl;
		//cout << "M Bounds (min, max):" << dBounds[6] << ", " << dBounds[7] << endl;
		//cout << endl;
		
		// Now we figure out the info inside!
		if (sShapeType == 5)
		{
			int iMainOffset = 100;
			int iRecordOffset = 0;
			
			//We are going to need the file opened now!
			
			string lsFileKML(lsFilePath + lsFileName + ".kml");
			
			//Declare the file and open it
			char * lcFileKML = new char[lsFileKML.size()];
			strcpy(lcFileKML,lsFileKML.c_str());
			
			ofstream outFile;
			outFile.open (lcFileKML);
			
			// Loop Through the document until it's over!
			//cout << "size = " << size << endl;
			
			//Create the Header!
			outFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<kml xmlns=\"http://earth.google.com/kml/2.2\">\n<Document>\n\t<name>";
			outFile << lsFileName;
			outFile << "</name>\n\t";
			outFile << "<Style id=\"stdstyle\">\n\t\t";
			outFile << "<IconStyle>\n\t\t\t<scale>1.1</scale>\n\t\t\t<Icon>\n\t\t\t\t<href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>\n\t\t\t</Icon>\n\t\t\t<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n\t\t</IconStyle>\n\t\t";
			outFile << "<LineStyle>\n\t\t\t<color>cc0300ff</color>\n\t\t</LineStyle>\n\t";
			outFile << "<PolyStyle>\n\t\t\t<color>AA03ff03</color>\n\t\t</PolyStyle>\n\t";
			outFile << "</Style>\n\t";
			outFile << "<StyleMap id=\"msn_ylw_pushpin\">\n\t\t\t<Pair>\n\t\t\t<key>normal</key>\n\t\t\t<styleUrl>#sn_ylw-pushpin</styleUrl>\n\t\t</Pair>\n\t\t<Pair>\n\t\t\t<key>highlight</key>\n\t\t\t<styleUrl>#sh_ylw-pushpin</styleUrl>\n\t\t</Pair>\n\t</StyleMap>\n\t<Style id=\"sh_ylw-pushpin\">\n\t\t<IconStyle>\n\t\t\t<scale>1.3</scale>\n\t\t\t<Icon>\n\t\t\t\t<href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>\n\t\t\t</Icon>\n\t\t\t<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n\t\t</IconStyle>\n\t\t<LineStyle>\n\t\t\t<color>cc0300ff</color>\n\t\t</LineStyle>\n\t</Style>\n";
			
			
			for (int iShapeCount=0; iMainOffset < size; iShapeCount++)
			{
				
				/*__int32 sRecordNumber = (
				 lcFileContents[iMainOffset+0] * 256 * 256 * 256
				 + lcFileContents[iMainOffset+1] * 256 * 256
				 + lcFileContents[iMainOffset+2] * 256
				 + lcFileContents[iMainOffset+3]
				 );*/
				__int32 sRecordLength = (
										 lcFileContents[iMainOffset+4] * 256 * 256 * 256
										 + lcFileContents[iMainOffset+5] * 256 * 256
										 + lcFileContents[iMainOffset+6] * 256
										 + lcFileContents[iMainOffset+7]
										 );
				sRecordLength = sRecordLength * 2;
				int iRecLen = 0;
				
				__int32 sInnerShapeType = (
										   lcFileContents[iMainOffset+8]
										   + lcFileContents[iMainOffset+9] * 256
										   + lcFileContents[iMainOffset+10] * 256 * 256
										   + lcFileContents[iMainOffset+11] * 256 * 256 * 256
										   );
				
				//cout << " ------------------------------------ " << endl;
				//cout << "Shape #" << sRecordNumber << " Type #" << sInnerShapeType << endl;
				//cout << "sRecordLength:" << sRecordLength << " - (" << (sRecordLength+4) << " bytes)" << endl;
				//cout << "sInnerShapeType:" << sInnerShapeType << endl;
				//cout << endl;
				//cout << "iMainOffset = " << iMainOffset << endl;
				//cout << "iMainOffset + sRecordLength " << iMainOffset + sRecordLength << endl;
				
				//Currently this program only supports shapefiles containing polygons
				if (sInnerShapeType == sShapeType)
				{
					//MBR, Number of parts, Number of points, Parts, Points
					double dShapeBounds[4];
					bool lbFindEnd = false;
					for (int iCount=0; iCount<4; iCount++)
					{
						double dCopy;
						int iOffset = iMainOffset+12+(iCount*8);
						memcpy(&dCopy, lcFileContents + iOffset, 8);
						dShapeBounds[(iCount)] = dCopy;
					}
					//cout << "Shape #" << sRecordNumber << " bounds are:" << endl;
					//cout << "My Shape #" << iShapeCount << endl; 
					//cout << "north: " <<  dShapeBounds[3] << " - south: " <<  dShapeBounds[1] << " - east: " <<  dShapeBounds[2] << " - west: " <<  dShapeBounds[0] << endl;
					
					__int32 sNumberOfParts = (
											  lcFileContents[iMainOffset+44]
											  + lcFileContents[iMainOffset+45] * 256
											  + lcFileContents[iMainOffset+46] * 256 * 256
											  + lcFileContents[iMainOffset+47] * 256 * 256 * 256
											  );
					//cout << "sNumberOfParts:" << sNumberOfParts << endl;
					
					__int32 sNumberOfPoints = (
											   lcFileContents[iMainOffset+48]
											   + lcFileContents[iMainOffset+49] * 256
											   + lcFileContents[iMainOffset+50] * 256 * 256
											   + lcFileContents[iMainOffset+51] * 256 * 256 * 256
											   );
					
					if (sNumberOfPoints<0){sNumberOfPoints=sNumberOfPoints*-1;}
					iRecLen = ((44+(sNumberOfParts*4)+(sNumberOfPoints*16)) - sRecordLength);
					iRecordOffset = 52+(sNumberOfParts*4)+(sNumberOfPoints*16);
					//Skip out if it's an empty shape
					if (sNumberOfPoints < 1)
					{
						break;
					}
					
					__int32 * sPartBegin = new __int32[sNumberOfParts];
					for (int iCount=0; iCount<sNumberOfParts; iCount++)
					{
						int iOffset = iMainOffset+52+(iCount*4);
						sPartBegin[iCount] = (
											  lcFileContents[iOffset+0]
											  + lcFileContents[iOffset+1] * 256
											  + lcFileContents[iOffset+2] * 256 * 256
											  + lcFileContents[iOffset+3] * 256 * 256 * 256
											  );
					}
					
					
					//Find the points for the polygon of the current part
					double sPoints[2];
					int iPartCount=0;
					for (int iCount=0; iCount<sNumberOfPoints; iCount++)
					{
						int iOffset = iMainOffset+52+(sNumberOfParts*4)+(iCount*16);
						double dCopy;
						memcpy(&dCopy, lcFileContents + iOffset, 8);
						sPoints[0] = dCopy;
						memcpy(&dCopy, lcFileContents + iOffset+8, 8);
						sPoints[1] = dCopy;		
						
						if (iPartCount < sNumberOfParts)
						{
							if (iCount == (sPartBegin[iPartCount]))
							{
								
								if (iCount > 0)
								{
									//Write the shape out!
									outFile << "\t\t\t\t\t</coordinates>\n\t\t\t\t</LinearRing>\n\t\t\t\t</outerBoundaryIs>\n\t\t\t</Polygon>" << "\n";
								}
								
								std::stringstream sStringBuffer;
								sStringBuffer << "Shape " << (iShapeCount+1);
								string lsName(sStringBuffer.str());
								std::stringstream sStringBufferDescs;
								string lsDescription(lsName);
								
								///////////////////////
								//Read the DBF File
								if (lbDBFFound)
								{
									
									int iLastEndPoint = iShapeCount*DBFRecordLength;
									for (int c=0; c<((DBFHeaderLength-31) / 32); c++)
									{
										std::stringstream sStringBufferColVal;
										std::stringstream sStringBufferColName;
										__int32 iColumnLen = lcFileContentsDBF[48+(c*32)];//
										
										//////////////////////////////////////////////////////////////
										//Get Column Name
										
										sStringBufferColName <<	lcFileContentsDBF[32+c*32] <<  
										lcFileContentsDBF[33+c*32] <<  
										lcFileContentsDBF[34+c*32] <<  
										lcFileContentsDBF[35+c*32] <<  
										lcFileContentsDBF[36+c*32] <<  
										lcFileContentsDBF[37+c*32] <<  
										lcFileContentsDBF[38+c*32] <<  
										lcFileContentsDBF[39+c*32] <<  
										lcFileContentsDBF[40+c*32] <<  
										lcFileContentsDBF[41+c*32] <<  
										lcFileContentsDBF[42+c*32];
										
										//////////////////////////////////////////////////////
										
										
										string sColName(sStringBufferColName.str());
										for (int i=0; i<iColumnLen; i++)
										{
											if (sizeDBF > (DBFHeaderLength+1+iLastEndPoint))
											{
												sStringBufferColVal << lcFileContentsDBF[(DBFHeaderLength+1+iLastEndPoint)];
											}
											iLastEndPoint++;
										}
										std::stringstream sStringBufferColDesc;
										string sColVal(sStringBufferColVal.str());
										TrimSpaces(sColVal);
										TrimSpaces(sColName);
										sStringBufferColDesc << sColName << ": " << sStringBufferColVal.str() << endl;
										string lsOutput(sStringBufferColDesc.str());
										TrimSpaces(lsOutput);
										if (c==iNameCol)
										{
											lsName = sColVal;
										}
										if (c!=0) {sStringBufferDescs << endl;}
										sStringBufferDescs << lsOutput;
									}
								}
								//////////////////////////////
								
								string lsFullDesc(sStringBufferDescs.str());
								replace(lsName.begin(), lsName.end(), '&', '+');
								replace(lsFullDesc.begin(), lsFullDesc.end(), '&', '+');
								lsDescription = lsFullDesc;
								
								if (iPartCount==0)
								{
									outFile << "\t<Placemark>" << "\n";
									outFile << "\t\t<name>";
									outFile << lsName;
									outFile << "</name>" << "\n";
									outFile << "\t\t<description>";
									outFile << lsDescription;
									outFile << "</description>" << "\n";
									outFile << "\t\t<styleUrl>#stdstyle</styleUrl>\n\t\t\t";
									outFile << "\t\t<MultiGeometry>\n\t\t\t";
								}
								outFile << "<Polygon>\n\t\t\t<tessellate>1</tessellate>\n\t\t\t\t<outerBoundaryIs>\n\t\t\t\t<LinearRing>" << "\n";
								outFile << "\t\t\t\t\t<coordinates>" << "\n";
								iPartCount++;
							}
						}
						
						
						//THIS IS WHERE THE STATEPLANE CONVERSION CODE CAN BE REFERENCED
						//double fLong = (sPoints[0]);
						//double fLati = (sPoints[1]);
						//double foLon = 0;
						//double foLat = 0;
						
						//ConvertStatePlane(fLong, fLati, foLon, foLat);
						
						outFile << "\t\t\t\t\t\t" << setprecision(21) << sPoints[0] << ", " << sPoints[1] << ",0" << "\n";
						if (iCount == sNumberOfPoints-1 && lbFindEnd)
						{
							//In some cases we just keep searching for the end of the file
							sNumberOfPoints++;
						}
						
					}
					outFile << "\t\t\t\t\t</coordinates>\n\t\t\t\t</LinearRing>\n\t\t\t\t</outerBoundaryIs>\n\t\t\t</Polygon>\n\t\t\t</MultiGeometry>\n\t</Placemark>" << "\n";
					iMainOffset = iMainOffset+ iRecordOffset; 
				}
				else
				{
					//If for some reason a shape is corrupt, we can just pass through it bit by bit to the next shape so the file isn't a compete waste
					iMainOffset++; //Slow, and crappy way to bypass things that we don't like!
				}
				
			}
			if (size > iMainOffset)
			{
				cout << "incomplete file conversion!" << endl;
				cout << "File finished at point: " << iMainOffset << " size is: " << size << endl;
			}
			else
			{
				cout << "completed" << endl;
			}
			outFile << "</Document>\n</kml>" << "\n";
			outFile.close();
			
		}		
		
		// We're done, delete the file from memory!
		
		if (lbDBFFound)
		{
			delete[] lcFileContentsDBF;
		}
		
		delete[] lcFileContents;
		liReturnValue = 1;
	}
	else 
	{
		cout << "Unable to open file";
	}
	return liReturnValue;
}



int main (int argc, char * const argv[]) {
	int liReturnValue = 0;
	
	//Ask for shapefile file name
	
	for (int i=1; i<argc; i++)
	{
		//cout << argv[i];
		
		string str = argv[i];
		size_t found;
		size_t found2;
		found=str.find_last_of("/\\");
		found2=str.find_last_of(".");
		//cout << " folder: " << str.substr(0,found+1) << endl;
		//cout << " file: " << str.substr(found+1,found2-found-1) << endl;
		//cout << " ext: " << str.substr(found2+1) << endl;
		
		string lsFileName = str.substr(found+1,found2-found-1);
		string lsFilePath = str.substr(0,found+1);
		
		int liFile1 = liOpenFile(lsFileName, lsFilePath);
		if (liFile1>0)
		{
			int iCol = 0;
			cout << "-----------------------------" << endl;
			cout << "Which Column would you like to use for the Title field? ";
			cin >> iCol;
			liReturnValue = liLoadToMemory(lsFileName, lsFilePath, iCol-1);
		}
		
	}
	
    return liReturnValue;
}