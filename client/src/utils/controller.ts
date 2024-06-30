export enum ControllerMessageType {
  ALARM = 'esp32-alarm: ',
  PH_VALUE = 'esp32-pH-value: '
}

export const checkMessage = (message: string, type: ControllerMessageType): boolean => {
  return message.includes(type)
}

export const formatMessage = (message: string): string => {
  const messageValues = Object.values(ControllerMessageType)
  for (const value of messageValues) {
    if (checkMessage(message, value)) {
      return message.replace(value, '')
    }
  }
  return ''
}