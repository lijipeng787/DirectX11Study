
// Filename: texturearrayclass.h

#ifndef _TEXTUREARRAYCLASS_H_
#define _TEXTUREARRAYCLASS_H_





#include <d3d11.h>
#include <DDSTextureLoader.h>
using namespace DirectX;



// Class name: TextureArrayClass

class TextureArrayClass
{
public:
	TextureArrayClass();
	TextureArrayClass(const TextureArrayClass&);
	~TextureArrayClass();

	bool Initialize(WCHAR*, WCHAR*, WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView** GetTextureArray();

private:
	ID3D11ShaderResourceView* textures_[3];
};

#endif