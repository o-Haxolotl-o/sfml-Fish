uniform sampler2D texture;

void main()
{
    // lookup the pixel in the texture
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);


    if(pixel.a < .8)
    {
        gl_FragColor = 0;
    }
    else
    {
        gl_FragColor = 1;
    }
}

