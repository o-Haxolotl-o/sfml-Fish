uniform sampler2D texture;

void main()
{
    // lookup the pixel in the texture
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);


    if(pixel.a < .8) //clear
    {
        gl_FragColor = 0;
    }
    /*else if(pixel.a < .85) // outline
    {
        gl_FragColor = vec4(.5, .2, 1, 1);
    }*/
    else //fill
    {
        gl_FragColor = vec4(pixel.r, pixel.g, pixel.b, 1);
    }
}

