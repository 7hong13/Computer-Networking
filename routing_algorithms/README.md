# Routing Algorithms
Routing algorithms implementation in C
<br> ([original repository](https://github.com/7hong13/Routing-Algorithm))
<br>
<br>

## Distance Vector
- location: distvec/distvec.cc
- execution: ./distvec topologyfile messagesfile changesfile 
<br> (e.g. ./distvec topology.txt messages.txt changes.txt)
<br>

## Link State
- location: ~~linkstate/linkstate.cc~~ (to be revised)
- execution: ./linkstate topologyfile messagesfile changesfile
<br> (e.g. ./linkstate topology.txt messages.txt changes.txt)
<br>

## Input files (for both)
- topoloyfile: txt format, contains information about nodes and links
- messagesfile: txt format, includes messages to send
- changesfile: txt format, contains state changes on links
