import { fastify } from 'fastify'
import { spawn } from 'child_process'

const server = fastify()

const executeProgram = (filePath, args = [], input = '') => {
    return new Promise((resolve, reject) => {
        const process = spawn(filePath, args)
        let output = ''
        let errorOutput = ''

        process.stdout.on('data', (data) => {
            output += data.toString()
        })

        process.stderr.on('data', (data) => {
            errorOutput += data.toString()
        })

        process.on('error', (error) => {
            reject(`Error: ${error.message}`)
        })

        process.on('close', (code) => {
            if (code === 0 && !errorOutput) {
                resolve(output)
            } else {
                reject(`Process exited with code ${code}, stderr: ${errorOutput}`)
            }
        })

        if (input) {
            process.stdin.write(input)
            process.stdin.end()
        }
    })
}

server.get('/', async (request, reply) => {
    try {
        const output = await executeProgram('./home.exe')
        reply.send(output)
    } catch (error) {
        reply.status(500).send(error)
    }
})

server.get('/station/:value', async (request, reply) => {
    const { value } = request.params
    try {
        const output = await executeProgram('./station.exe', [], value)
        reply.send(output)
    } catch (error) {
        reply.status(500).send(error)
    }
})

server.get('/temperature/:value', async (request, reply) => {
    const { value } = request.params
    try {
        const output = await executeProgram('./temperature.exe', [], value)
        reply.send(output)
    } catch (error) {
        reply.status(500).send(error)
    }
})

server.listen({
    port: 7070,
}, (err, address) => {
    if (err) {
        console.error(err)
        process.exit(1)
    }
    console.log(`Server listening at ${address}`)
})
