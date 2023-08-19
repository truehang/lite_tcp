# Principal
- Keep code self-explanatory and reader-friendly. Please add comments for tricky part.  
- Try to keep code concise.
- Try to use SOLID design principle: single responsibility, open closed, Liskov substitution, interface segregation, dependency inversion.
# Format
Using Google C++ style, remember to style your code before commitment.   
Recommend use clang-format tool to style your code, the style config is in .clang-format file. For example I want to style source files under include and src, use:
```bash
clang-format include/*.hpp src/*.cpp -style=file -i
```
