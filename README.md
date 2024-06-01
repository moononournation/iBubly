# AVI Player

A simple AVI player for Arduino IDE.

## conversion

### MJPEG

```sh
ffmpeg -i input.mp4 -c:a mp3 -c:v mjpeg -q:v 4 -vf "fps=30,scale=-1:240:flags=lanczos,crop=280:240:(in_w-280)/2:0" AviMp3Mjpeg240p30fps.avi
```
