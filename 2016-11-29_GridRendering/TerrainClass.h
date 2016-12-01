#ifndef _TERRAINCLASS_H_
#define _TERRAINCLASS_H_

#include <d3d11.h>
#include <d3dx10math.h>

class TerrainClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
	};

public:
	TerrainClass();
	TerrainClass(const TerrainClass &);
	~TerrainClass();

	bool Initialze(ID3D11Device *);
	void Shutdonw();
	void Render(ID3D11DeviceContext *);

	int GetIndexCount();

private:
	bool InitialzeBuffers(ID3D11Device *);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext *);

private:
	int m_terrainWidth;
	int m_terrainHeight;

	int m_vertexCount;
	int m_indexCount;

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
};

#endif
