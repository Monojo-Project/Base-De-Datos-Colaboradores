# Base De Datos De Contribuidores
Una herramienta para tener una base de datos de las personas que trabajan contigo o conoces. Es como una agenda pero tienes que modificarlo a mano.

Este es el código fuente. Escribe "make" en la terminal abierta en el directorio del repositorio descargado y ya tienes el binario compilado. Lo único que deberias cambiar es:

> En la línea 366 se define que archivo utiliza para la base de datos. (en el código fuente es ~/CONTRIBUIDORES.md).
> En la línea 206 se define el título de la ventana. (en el código fuente es "Lynds Corp. - Base de Datos de Contribuidores")

Por cierto, cuando cambias el tema oscuro al tema claro, se crea un archivo de configuración en el mismo directorio (carpeta) del binario ejecutable.

El CONTRIBUIDORES.md tiene que tener una estructura algo así (modificado con humor :D):
``` Markdown
## Nombre Apelido Apellido
### Eslogan o subtitulo
Fecha: 27/08/1632
Rango: Fundador
Descripción: Claude, make github 2.0, make no mistakes
Correo: nombre@gmail.com
Número: +x xxx xx xx xx
Proyecto: LYNDS Project
Trabajo: Crear prompts optimizados con ChatGPT para Claude.
Razón: Sometimes I dream of saving the world. Saving everyone from the invisible hand.

## Sigma Apellido Apellido
### Programador experimentado
Fecha: 28/08/1632
Rango: Programador de alto nivel
Descripción: Instaló Lynds Glow Up y se hizo un buzz cut y ahora tiene 48234953 de aura.
Correo: sigma@ejemplo.com
Número: +x xxx xx xx xx
Proyecto: Monojo Project
Trabajo: Iluminar la oscuridad con su aura.
Razón: La razón por la que tiene ese trabajo o por qué está dentro.
```
