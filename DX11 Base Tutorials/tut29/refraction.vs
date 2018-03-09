
// Filename: refraction.vs






cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer ClipPlaneBuffer
{
    float4 clipPlane;
};





struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
   	float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float clip : SV_ClipDistance0;
};





PixelInputType RefractionVertexShader(VertexInputType input)
{
    PixelInputType output;
    


    input.position.w = 1.0f;

	
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;
    
	// Calculate the normal vector against the world matrix only.
	output.normal = mul(input.normal, (float3x3)worldMatrix);
	
	// Normalize the normal vector.
	output.normal = normalize(output.normal);
	
	// Set the clipping plane.
    output.clip = dot(mul(input.position, worldMatrix), clipPlane);

    return output;
}