error:avfilter_graph_create_filter() return -22
buffersrc_ctx的初始化需要args作为参数，否则无法正常初始化。
args为描述输入帧序列的参数信息

Fontconfig error: Cannot load default config file
出现这个说明字体没有配置好，windows平台下需如下配置
FONTCONFIG_FILE=font.conf
FONTCONFIG_PATH=C:\ffmpeg
将fonts.conf文件放至上述指定目录。








