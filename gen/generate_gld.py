import xml.etree.ElementTree as ET

tree = ET.parse('gl.xml')
registry = tree.getroot()
text = ""

for commands_block in registry.findall('commands'):
    for command in commands_block.findall('command'):
        proto = command.find('proto')
        gl_name = proto.find('name').text
        gld_name = 'gld' + gl_name[2:]
        ptype = proto.find('ptype')
        params = command.findall('param')
        param_names = [p.find('name').text for p in params]
        wrapped_args_text = ', '.join(f"({param_name})" for param_name in param_names)
        if ptype is None:
            args_text = f"{', '.join(param_names)}"
            line = f"#define {gld_name}({args_text}) GLD_CALL({gl_name}({wrapped_args_text}))"
        else:
            args_text = ", ".join(['sret'] + param_names)            
            line = f"#define {gld_name}({args_text}) GLD_CALL_RET(sret, {gl_name}({wrapped_args_text}))"

        print(line)


                
