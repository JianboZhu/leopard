leopard
======

leopard is an online debug framework.

leopard is an lightweight, functional and embeddable online debug frameworkt to make it easy for developpers to 
provide an online denbugging feature for their aplications.

leopard is very easy to use by embedding it into your application. First, get a global and thread safe http server instance; Secondly, start it. Thirdly, register your callback function
with specifying a path. At last, stop it in somewhere you stop your application. For detail infomation, please see 'example' part in the project.

Note:
* leopard is only working on linux platform now.
* Rabibit uses 'blade', an advanded building system, as its building tool.
* It is thread safe.
* leopard use 'mongoose' as its default http server in cpp version. you can replace it by implementing 'AbstractHttpServer'.
* leopard is lightweight. 
