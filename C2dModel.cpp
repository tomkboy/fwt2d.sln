#include "stdafx.h"
#include "C2dModel.h"
#include <string>

/******************完全匹配层法PML处理边界**************************/
void C2dModel::ConstVelocity()
{
	m_vel.assign(nn(),2500.0f);
	/*for (int i=0;i<m_nx;i++)
	{
		for (int j=30;j<60;j++)
		{
			m_vel[GetUstore(i,j)] = 2000.0f;
		}
		for (int j=60;j<m_nz;j++)
		{
			m_vel[GetUstore(i,j)] = 2400.0f;
		}
	}*/

	
	/*for (int j=0;j<15;j++)
	{
		for (int i=15-j;i<15+j+1;i++)
		{
			m_vel[GetUstore(i,j+5)] = 1800.0f;
		}		
	}*/

// 	for (int j = 0; j < m_nz-10; j++)
// 	{
// 		for (int i = 0; i < 5; i++)
// 		{
// 			int i1 = (m_nx-j-2+i)/2;
// 			int i2 = m_nx  - i1;
// 			m_vel[GetUstore(i1,j+5)] = 5000.0f;
// 			m_vel[GetUstore(i2,j+5)] = 5000.0f;
// 		}		
// 	}
}

void C2dModel::IncreaseVelocity()
{
	//float min = 1500.0f, max = 4000.0f;
	//float min = 1400.0f, max = 4200.0f;
	 /// float min = 3000.0f, max = 4000.0f;
	float min = 2000.0f, max = 3000.0f;
	float step = (max - min) / m_nz;
	//m_vel.assign(m_nn, 1000);
	for (int i=0;i<m_nx;i++)
	{
		for (int j=0;j<m_nz;j++)
		{
			m_vel[GetUstore(i,j)] = min + step*j;
		}
	}
}

void C2dModel::HighVelocity()
{
	for (int i=0;i<m_nx;i++)
	{
		for (int j=0;j<m_nz;j++)
		{
			m_vel[GetUstore(i,j)] = 2500.0f;// 原来为3500
		}
	}
	for (int i=2*m_nx/5;i<3*m_nx/5;i++)
	{
		for (int j=2*m_nz/5;j<3*m_nz/5;j++)
		{
			m_vel[GetUstore(i,j)] = 2700.0f;  //原来为3700
		}
	}
}

void C2dModel::ThreeLayer()     //三层速度模型
{
	m_vel.assign(nn(),0.0f);    //use in forward one shot
	for (int i=0;i<m_nx;i++)
	{
		for (int j=0;j<m_nz;j++)
		{
			if(j<30)
				m_vel[GetUstore(i,j)] = 2000.0f;
			else if(j<60)
				m_vel[GetUstore(i,j)] = 2500.0f;
			else 
				m_vel[GetUstore(i,j)] = 3000.0f;   
			/*if(j<30)
				m_vel[GetUstore(i,j)] = 2000.0f;
			else if(j<60)
				m_vel[GetUstore(i,j)] = 2500.0f;
			else 
				m_vel[GetUstore(i,j)] = 3000.0f; //90深度的三层模型
				*/
			/*if(j<20)
				m_vel[GetUstore(i,j)] = 2000.0f;
			else if(j<40)
				m_vel[GetUstore(i,j)] = 2300.0f;
			else if(j<60)
				m_vel[GetUstore(i,j)] = 2600.0f;
			else if(j<80)
				m_vel[GetUstore(i,j)] = 2900.0f;
			else m_vel[GetUstore(i,j)] = 3000.0f;*/  // 90深度五层模型

			/*if(j<80) 
				m_vel[GetUstore(i,j)] = 2000.0f;
			else if(j<160)
				m_vel[GetUstore(i,j)] = 2500.0f;
			else
				m_vel[GetUstore(i,j)] = 3000.0f;*/     //240深度的三层模型
			
			/*if(j<30)
			    m_vel[GetUstore(i,j)] = 1400.0f;
			else if(j<60)
			    m_vel[GetUstore(i,j)] = 1800.0f;
			else if(j<90)
			    m_vel[GetUstore(i,j)] = 2200.0f;
			else if(j<120)
			    m_vel[GetUstore(i,j)] = 2600.0f;
			else if(j<150)
			    m_vel[GetUstore(i,j)] = 3000.0f;
			else if(j<180)
			    m_vel[GetUstore(i,j)] = 3400.0f;
			else if(j<210)
				m_vel[GetUstore(i,j)] = 3800.0f;
			else
				m_vel[GetUstore(i,j)] = 4200.0f;*/    //240深度的八层模型
			     
		}
	}
}

int C2dModel::TranIndex(int i)   //不加PML转换成加PML
{
	int i1 = i/m_nz + m_npmlx;
	int i2 = i%m_nz + m_npmlz;		
	return i1*nze() + i2;
}

void C2dModel::OpenVelocity(const string &VelName)
{
	FILE * pFile;
	pFile = fopen (VelName.c_str(),"rb");
	if (pFile == NULL)
	{
		AfxMessageBox("file open for read fail");		
	}
	m_vel.assign(nn(), 0);
	fread(&m_vel[0], sizeof(float), nn(), pFile);
	fclose (pFile);
}

void C2dModel::SaveResult(string str, const vector<float> &result,int n)
{
	char buffer[10];	
	
	_itoa_s(n,buffer,10,10);	
	 str =  ".\\result\\"  + str;	
	//str =  "E:\\Codes\\backup\\fwt2d\\result"  + str;	
	str += buffer;
	str += ".bin";

	FILE * pFile;
	pFile = fopen (str.c_str(),"wb");
	if (pFile == NULL)
	{
		AfxMessageBox("file open fail");		
	}	
	fwrite(&result[0], sizeof(float), result.size(), pFile);
	fclose (pFile);
}

void C2dModel::Modetext(const vector<float> &v,float *ve)  //速度模型加PML扩展
{
	int i,j;
	for (i=0;i<m_nx;i++)
	{
		for (j=0;j<m_nz;j++)
		{
			ve[Get(i+m_npmlx,j+m_npmlz)]=v[GetUstore(i,j)];
		}
	}

	// right-left- sides
	for (i=0;i<m_npmlx;i++)
	{
		for (j=0;j<m_nz;j++)
		{
			ve[Get(i, j+m_npmlz)]=v[GetUstore(0,j)];
			ve[Get(i+m_nx+m_npmlx, j+m_npmlz)]=v[GetUstore(m_nx-1,j)];
		}
	}

	//	upper_low-sides
	for (i = 0; i < m_nx; i++)
	{
		for (j = 0; j < m_npmlz; j++)
		{
			ve[Get(i+m_npmlx, j)]=v[GetUstore(i,0)];
			ve[Get(i+m_npmlx, j+m_nz+m_npmlz)]=v[GetUstore(i,m_nz-1)];
		}
	}

	//corners
	for (i=0;i<m_npmlx;i++)
	{
		for (j=0;j<m_npmlz;j++)
		{
			ve[Get(i, j)]=v[GetUstore(0,0)];
			ve[Get(i+m_nx+m_npmlx, j+m_nz+m_npmlz)]=v[GetUstore(m_nx-1,m_nz-1)];
			ve[Get(i, j+m_nz+m_npmlz)]=v[GetUstore(0,m_nz-1)];
			ve[Get(i+m_nx+m_npmlx, j)]=v[GetUstore(m_nx-1,0)];
		}
	}	
}
