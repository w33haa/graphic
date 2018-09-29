#version 420 core
out vec4 FragColor;


uniform sampler2D fireTextureSampler;
uniform sampler2D noiseTextureSampler;
uniform sampler2D alphaTextureSampler;

/*
uniform DistortionBuffer
{
    vec2 distortion1;
    vec2 distortion2;
    vec2 distortion3;
    float distortionScale;
    float distortionBias;
}distortionParam;
*/


in VS_OUT {
    vec4 position;
    vec2 texCoords0;
    vec2 texCoords1;
    vec2 texCoords2;
    vec2 texCoords3;

} vs_in;

void main()
{
	// 写死先
	vec2 distortion1 = vec2(0.1f, 0.2f);
	vec2 distortion2 = vec2(0.1f, 0.3f);
	vec2 distortion3 = vec2(0.1f, 0.1f);
	float distortionScale =  0.8f;
	float distortionBias =  0.5f;


    vec4 noise1;
    vec4 noise2;
    vec4 noise3;
    vec4 finalNoise;
    float perturb;
    vec2 noiseCoords;
    vec4 fireColor;
    vec4 alphaColor;

	// 分别获取三张不同scale的噪声纹理中的texel
    noise1 = texture(noiseTextureSampler, vs_in.texCoords1);
    noise2 = texture(noiseTextureSampler, vs_in.texCoords2);
    noise3 = texture(noiseTextureSampler, vs_in.texCoords3);

    // Move the noise from the (0, 1) range to the (-1, +1) range.
    noise1 = (noise1 - 0.5f) * 2.0f;
    noise2 = (noise2 - 0.5f) * 2.0f;
    noise3 = (noise3 - 0.5f) * 2.0f;


    // 对三个噪声值变形
    noise1.xy = noise1.xy * distortion1.xy;
    noise2.xy = noise2.xy * distortion2.xy;
    noise3.xy = noise3.xy * distortion3.xy;

    // 形成最后的噪声
    finalNoise = noise1 + noise2 + noise3;

	// 对噪声进行扰动，火焰上方扰动较大，越往下扰动越小
    perturb = ((1.0f - vs_in.texCoords0.y) *distortionScale) + distortionBias;

    
    noiseCoords.xy = (finalNoise.xy * perturb) + vs_in.texCoords0.xy;


    // 用最后计算的噪声作为火焰颜色的纹理坐标
    fireColor = texture(fireTextureSampler, noiseCoords);
	//fireColor = texture(fireTextureSampler, vs_in.texCoords1);
	

    // 用alpha贴图控制火焰形状，通过设置颜色的alpha通道实现
    alphaColor = texture(alphaTextureSampler, noiseCoords);
    fireColor.a = alphaColor.r;
	//fireColor.a = 1.0;
	//if(fireColor.a < 0.5)
    //    discard;
	FragColor = fireColor;
    //FragColor =  texture(fireTextureSampler, vs_in.texCoords0);
}
